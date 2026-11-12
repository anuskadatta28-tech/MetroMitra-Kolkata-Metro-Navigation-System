"use strict";

const lineColors = {
  Blue: "#2874c8",
  Green: "#2da779",
  Purple: "#9161c8",
  Yellow: "#d9a72e",
  Orange: "#e68147"
};

const elements = {
  form: document.querySelector("#journey-form"),
  origin: document.querySelector("#origin"),
  destination: document.querySelector("#destination"),
  options: document.querySelector("#station-options"),
  message: document.querySelector("#form-message"),
  planButton: document.querySelector("#plan-button"),
  result: document.querySelector("#result-card"),
  lineList: document.querySelector("#line-list")
};

let network;

function parseCsv(text) {
  const [header, ...rows] = text.trim().split(/\r?\n/);
  const columns = header.split(",").map((column) => column.trim());
  return rows.filter(Boolean).map((row) => {
    const values = row.split(",");
    return Object.fromEntries(columns.map((column, index) => [column, values[index]?.trim() ?? ""]));
  });
}

async function loadNetwork() {
  const files = ["stations.csv", "lines.csv", "connections.csv", "interchanges.csv"];
  const datasets = await Promise.all(files.map(async (file) => {
    const response = await fetch(`../dataset/${file}`);
    if (!response.ok) {
      throw new Error(`Could not load ${file} (${response.status}).`);
    }
    return parseCsv(await response.text());
  }));
  const [stations, lines, connections, interchanges] = datasets;
  return buildNetwork(stations, lines, connections, interchanges);
}

function buildNetwork(stations, lines, connections, interchanges) {
  const nodes = [];
  const nodeIndex = new Map();
  const adjacency = [];
  const addNode = (stationId, lineId) => {
    const key = `${stationId}:${lineId}`;
    if (!nodeIndex.has(key)) {
      nodeIndex.set(key, nodes.length);
      nodes.push({ stationId, lineId });
      adjacency.push([]);
    }
    return nodeIndex.get(key);
  };

  for (const connection of connections) {
    const from = addNode(connection.from_station_id, connection.line_id);
    const to = addNode(connection.to_station_id, connection.line_id);
    const edge = {
      to,
      distance: Number(connection.distance_km),
      time: Number(connection.travel_time_min)
    };
    adjacency[from].push(edge);
    adjacency[to].push({ ...edge, to: from });
  }

  for (const interchange of interchanges) {
    const from = nodeIndex.get(`${interchange.station_id}:${interchange.from_line_id}`);
    const to = nodeIndex.get(`${interchange.station_id}:${interchange.to_line_id}`);
    if (from !== undefined && to !== undefined) {
      adjacency[from].push({
        to,
        distance: 0,
        time: Number(interchange.transfer_time_min)
      });
    }
  }

  return {
    stations,
    lines,
    connections,
    nodes,
    adjacency,
    stationByName: new Map(stations.map((station) => [station.station_name.toLocaleLowerCase(), station])),
    stationById: new Map(stations.map((station) => [station.station_id, station])),
    lineById: new Map(lines.map((line) => [line.line_id, line]))
  };
}

function renderNetwork() {
  document.querySelector("#station-count").textContent = network.stations.length;
  document.querySelector("#line-count").textContent = network.lines.length;
  document.querySelector("#connection-count").textContent = network.connections.length;
  elements.options.replaceChildren(...network.stations.map((station) => {
    const option = document.createElement("option");
    option.value = station.station_name;
    return option;
  }));
  elements.lineList.replaceChildren(...network.lines.map((line) => {
    const chip = document.createElement("span");
    chip.className = "line-chip";
    chip.innerHTML = `<span class="line-chip-dot" style="--line-color:${escapeAttribute(line.line_color)}"></span>${escapeHtml(line.line_name)}`;
    return chip;
  }));
}

function findFastestRoute(originId, destinationId) {
  const distances = Array(network.nodes.length).fill(Infinity);
  const parents = Array(network.nodes.length).fill(-1);
  const queue = [];
  network.nodes.forEach((node, index) => {
    if (node.stationId === originId) {
      distances[index] = 0;
      queue.push([0, index]);
    }
  });

  while (queue.length) {
    queue.sort((a, b) => a[0] - b[0]);
    const [time, current] = queue.shift();
    if (time !== distances[current]) continue;
    if (network.nodes[current].stationId === destinationId) {
      const path = [];
      for (let node = current; node !== -1; node = parents[node]) path.push(node);
      return path.reverse();
    }
    for (const edge of network.adjacency[current]) {
      const nextTime = time + edge.time;
      if (nextTime < distances[edge.to]) {
        distances[edge.to] = nextTime;
        parents[edge.to] = current;
        queue.push([nextTime, edge.to]);
      }
    }
  }
  return [];
}

function getRouteDetails(path) {
  let distance = 0;
  let time = 0;
  let transfers = 0;
  const stops = [];

  path.forEach((nodeIndex, index) => {
    const node = network.nodes[nodeIndex];
    if (index === 0 || network.nodes[path[index - 1]].stationId !== node.stationId) {
      stops.push({ ...node, station: network.stationById.get(node.stationId) });
    }
    if (index === 0) return;
    const previous = network.nodes[path[index - 1]];
    const edge = network.adjacency[path[index - 1]].find((item) => item.to === nodeIndex);
    if (edge) {
      distance += edge.distance;
      time += edge.time;
    }
    if (previous.lineId !== node.lineId) transfers += 1;
  });

  return { distance, time, transfers, stops };
}

function renderRoute(path) {
  const details = getRouteDetails(path);
  const origin = details.stops[0].station.station_name;
  const destination = details.stops[details.stops.length - 1].station.station_name;
  const fare = Math.ceil(details.distance / 2) * 5;
  const listItems = [];

  details.stops.forEach((stop, index) => {
    const line = network.lineById.get(stop.lineId);
    const color = lineColors[line.line_color] ?? "#2874c8";
    const label = index === 0 ? "Start" : index === details.stops.length - 1 ? "Destination" : "";
    listItems.push(
      `<li class="route-stop" style="--stop-color:${escapeAttribute(color)}">
        <span class="route-dot" aria-hidden="true"></span>
        <span class="stop-name">${escapeHtml(stop.station.station_name)}</span>
        <span class="stop-meta">${label}</span>
      </li>`
    );
    const next = details.stops[index + 1];
    if (next && next.lineId !== stop.lineId) {
      const nextLine = network.lineById.get(next.lineId);
      const transfer = network.adjacency[path.findIndex((nodeIndex) => network.nodes[nodeIndex].stationId === stop.station.station_id && network.nodes[nodeIndex].lineId === stop.lineId)] ?? [];
      const transferEdge = transfer.find((edge) => network.nodes[edge.to].stationId === stop.station.station_id && network.nodes[edge.to].lineId === next.lineId);
      listItems.push(`<li class="transfer-note">Change to ${escapeHtml(nextLine.line_name)}${transferEdge ? ` · ${transferEdge.time} min transfer` : ""}</li>`);
    }
  });

  elements.result.innerHTML = `
    <div class="result-header">
      <div><p class="eyebrow">YOUR FASTEST ROUTE</p><div class="result-title">${escapeHtml(origin)} <span aria-hidden="true">→</span> ${escapeHtml(destination)}</div></div>
      <span class="fastest-badge"><svg viewBox="0 0 16 16" fill="none" aria-hidden="true"><path d="m8 1 1.7 4.7 4.8 1.8-4.8 1.7L8 14l-1.7-4.8L1.5 7.5l4.8-1.8L8 1Z" fill="currentColor"/></svg>Fastest</span>
    </div>
    <div class="trip-stats">
      <div class="trip-stat"><span class="trip-stat-value">${details.time} min</span><span class="trip-stat-label">estimated travel time</span></div>
      <div class="trip-stat"><span class="trip-stat-value">${formatDistance(details.distance)} km</span><span class="trip-stat-label">total distance</span></div>
      <div class="trip-stat"><span class="trip-stat-value">₹${fare}</span><span class="trip-stat-label">estimated fare</span></div>
    </div>
    <p class="route-label">YOUR JOURNEY · ${details.stops.length} STATIONS${details.transfers ? ` · ${details.transfers} TRANSFER${details.transfers > 1 ? "S" : ""}` : ""}</p>
    <ol class="route-list">${listItems.join("")}</ol>`;
  elements.result.hidden = false;
}

function formatDistance(distance) {
  return Number.isInteger(distance) ? distance.toFixed(1) : Number(distance.toFixed(2)).toString();
}

function escapeHtml(value) {
  return String(value).replace(/[&<>"']/g, (character) => ({
    "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;"
  })[character]);
}

function escapeAttribute(value) {
  return escapeHtml(value);
}

elements.form.addEventListener("submit", (event) => {
  event.preventDefault();
  if (!network) return;
  elements.message.textContent = "";
  const origin = network.stationByName.get(elements.origin.value.trim().toLocaleLowerCase());
  const destination = network.stationByName.get(elements.destination.value.trim().toLocaleLowerCase());
  if (!origin || !destination) {
    elements.message.textContent = "Choose a station from the suggestions to plan your journey.";
    elements.result.hidden = true;
    return;
  }
  if (origin.station_id === destination.station_id) {
    elements.message.textContent = "You’re already at your destination. Choose a different station.";
    elements.result.hidden = true;
    return;
  }
  const path = findFastestRoute(origin.station_id, destination.station_id);
  if (!path.length) {
    elements.result.innerHTML = `<p class="result-error">No route is available between ${escapeHtml(origin.station_name)} and ${escapeHtml(destination.station_name)} in the current dataset.</p>`;
    elements.result.hidden = false;
    return;
  }
  renderRoute(path);
  elements.result.scrollIntoView({ behavior: "smooth", block: "nearest" });
});

document.querySelector("#swap-button").addEventListener("click", () => {
  [elements.origin.value, elements.destination.value] = [elements.destination.value, elements.origin.value];
  elements.message.textContent = "";
});

document.querySelectorAll(".quick-picks button").forEach((button) => {
  button.addEventListener("click", () => {
    elements.origin.value = button.dataset.origin;
    elements.destination.value = button.dataset.destination;
    elements.form.requestSubmit();
  });
});

loadNetwork()
  .then((loadedNetwork) => {
    network = loadedNetwork;
    renderNetwork();
  })
  .catch((error) => {
    elements.message.textContent = "The metro data couldn’t be loaded. Start a local server from the project folder and open /web.";
    elements.planButton.disabled = true;
    console.error("Unable to load MetroMitra datasets:", error);
  });

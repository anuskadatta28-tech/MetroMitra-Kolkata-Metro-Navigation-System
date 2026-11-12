# MetroMitra web interface

The interface reads the project's CSV files from `dataset/` and calculates the
fastest route in the browser. Serve the repository root over HTTP, then open
`/web/` in your browser. For example, from the project root:

```powershell
python -m http.server 8000
```

Then visit `http://localhost:8000/web/`. Opening `index.html` directly as a
`file://` URL will not work because browsers block the CSV requests.

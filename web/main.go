package main

import (
	"embed"
	"io/fs"
	"log"
	"net/http"
)

//go:embed resources/*
var resources embed.FS

func main() {
	subFS, err := fs.Sub(resources, "resources")
	if err != nil {
		log.Fatal(err)
	}

	http.Handle("/", http.FileServer(http.FS(subFS)))

	http.HandleFunc("/widget", func(w http.ResponseWriter, r *http.Request) {
		http.ServeFile(w, r, "resources/index.html")
	})

	log.Println("Serving on http://localhost:8080")
	err = http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err)
	}
}

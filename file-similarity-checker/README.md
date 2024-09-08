# file-similarity-checker

Reads all file names in a directory and compares them.  
This allows you to for example find duplicate songs in a music collection, which often have slightly varying file names.  
The similarity of two file names is measured with the [Jaro-Winkler distance](https://en.wikipedia.org/wiki/Jaro%E2%80%93Winkler_distance).

This is a port of my [spotify-playlist-similarity-checker](https://github.com/3urobeat/spotify-playlist-similarity-checker) project.

## Start
`node index.js <path_to_directory>`

The script recursively fetches all file names in the specified directory. 

/*
 * File: readDirectory.js
 * Project: file-similarity-checker
 * Created Date: 2024-09-08 11:34:10
 * Author: 3urobeat
 *
 * Last Modified: 2024-09-08 11:36:29
 * Modified By: 3urobeat
 *
 * Copyright (c) 2024 3urobeat <https://github.com/3urobeat>
 *
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


const fs = require("fs");


module.exports.getFileNames = function(path) {

    // Helper function to scan directory recursively to get an array of all paths in this directory
    const scandir = function(dir) { // Credit for this function before I modified it: https://stackoverflow.com/a/16684530/12934162
        let results = [];
        const list = fs.readdirSync(dir);

        list.forEach(function(file) {
            const stat = fs.statSync(dir + "/" + file);

            results.push(file); // Push the file and folder in order to avoid an ENOTEMPTY error and push it before the recursive part in order to have the folder above its files in the array to avoid ENOENT error

            if (stat && stat.isDirectory()) results = results.concat(scandir(dir + "/" + file)); // Call this function recursively again if it is a directory
        });

        return results;
    };

    return scandir(path); // Scan the directory of this installation

};

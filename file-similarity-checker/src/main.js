/*
 * File: main.js
 * Project: file-similarity-checker
 * Created Date: 2022-04-22 19:46:18
 * Author: 3urobeat
 *
 * Last Modified: 2024-09-08 11:35:47
 * Modified By: 3urobeat
 *
 * Copyright (c) 2022 - 2024 3urobeat <https://github.com/3urobeat>
 *
 * Licensed under the MIT license: https://opensource.org/licenses/MIT
 * Permission is granted to use, copy, modify, and redistribute the work.
 * Full license information available in the project LICENSE file.
 */


const workerThreads = require("node:worker_threads");
const logger = require("output-logger");

const config = require("../config.json");

const { getFileNames } = require("./helpers/readDirectory.js");

const version = "1.0";


/**
 * Entry point
 */
module.exports.run = async () => {

    // Configure my logging library (https://github.com/3urobeat/output-logger#options-1)
    logger.options({
        msgstructure: `[${logger.Const.ANIMATION}] [${logger.Const.DATE} | ${logger.Const.TYPE}] ${logger.Const.MESSAGE}`,
        paramstructure: [logger.Const.TYPE, logger.Const.MESSAGE, "nodate", "remove", logger.Const.ANIMATION],
        outputfile: "./output.txt",
        animationdelay: 250,
        printdebug: false
    });

    global.logger = logger;


    // Log startup message
    logger("", "", true);
    logger("info", `file-similarity-checker v${version} by 3urobeat\n`);


    // Get directory path from argv
    const path = process.argv[2];

    if (!path) {
        logger("warn", "No input was given, exiting...");
        process.exit(1);
    }

    // Get all files at that path
    const data = getFileNames(path);


    // Display warning message if arr is large af
    if (data.length > 2000) logger("warn", `Comparing ${data.length} names will take a moment. Please be patient...`);

    let startTimestamp = Date.now();
    let workerPromises = [];

    logger("info", `Starting to compare ${data.length} names...`, false, false, logger.animation("loading"));

    // Create two arrays for collecting duplicates and non-duplicates with their similarity
    let duplicates = [];
    let similarities = [];

    // Spawn worker for every 50th element in the array to multithread this thing
    for (let i = 0; i < (data.length / 50); i++) {
        workerPromises.push(new Promise((resolve) => {
            const worker = new workerThreads.Worker("./src/helpers/compareStrings.js", {
                workerData: {
                    data,
                    config,
                    rangeStart: 50 * i,
                    rangeEnd: (50 * i) + 50
                }
            });

            // Get result from worker
            worker.once("message", (data) => {

                // Filter and add unique results
                for (e of data.dup) {
                    if (!duplicates.includes(e)) duplicates.push(e);
                }

                data.sim.forEach((e, i) => {
                    if (!similarities.some(f => e.compStr == f.compStr || e.compStr == f.compStrReversed)) similarities.push(e);

                    if (data.sim.length == i + 1) resolve(); // Mark this worker as done when all similarities have been processed
                });

            });
        }));

        // Start ready check on last iteration
        if (i + 1 >= (data.length / 50)) {
            await Promise.all(workerPromises);

            similarities.sort((a, b) => { return b.similarityPerc - a.similarityPerc; }); // Sort similarities descending by similarity in percent

            logger("info", "Finished comparing and sorting all names!", false, true);

            // Print results
            if (duplicates.length > 0) {
                let temp = "";
                duplicates.forEach((e) => temp += `+ ${e}\n`);

                logger("", "", true);
                logger("info", `I found ${duplicates.length} duplicate names:`);
                logger("", temp, true);
            } else {
                logger("info", "No duplicate names found!\n");
            }


            if (similarities.length > 0) {
                let temp = "";
                similarities.forEach((e) => temp += `+ ${e.compStr}\n`);

                logger("", "", true);
                logger("info", `I found ${similarities.length} similar names:`);
                logger("", temp, true);
            } else {
                logger("info", "No similar names found!\n");
            }


            let itTook = (Date.now() - startTimestamp) / 1000;

            logger("info", `Done after ${itTook} seconds!\n                             Check 'output.txt' for the full log. Exiting...\n`);
        }
    }
};

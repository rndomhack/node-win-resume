"use strict";

const Resume = require("../index");

let resume = new Resume({time: -60});   // Specify in seconds or Date object

resume.on("end", () => console.log("end"));
resume.on("error", err => console.log(`error: ${err}`));

resume.start();

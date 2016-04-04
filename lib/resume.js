"use strict";

const util = require("util");
const path = require("path");
const EventEmitter = require("events");
const child_process = require("child_process");

class Resume extends EventEmitter {
    constructor(options) {
        super();

        options = options || {};

        this.name = options.name || `Resume_${Math.random().toString().slice(2)}`;
        this.time = options.time || 0;
        this.cp = null;
    }

    set name(name) {
        if (typeof name !== "string") {
            throw new TypeError("Invalid name");
        }

        this._name = name;
    }

    get name() {
        return this._name;
    }

    set time(time) {
        if (!util.isDate(time) && !Number.isInteger(time)) {
            throw new TypeError("Invalid time");
        }

        this._time = time;
    }

    get time() {
        return this._time;
    }

    start() {
        if (this.cp) return false;

        let time;
        if (util.isDate(this.time)) {
            time = Math.floor(this.time.getTime() / 1000) + 11644473600;
        } else {
            time = this.time;
        }

        this.cp = child_process.spawn(path.join(__dirname, "../bin/resume"), [time, this.name]);

        this.cp.on("exit", () => {
            this.cp = null;
            this.emit("end");
        });

        this.cp.on("error", err => {
            this.cp = null;
            this.emit("error", err);
        });

        return true;
    }

    stop() {
        if (!this.cp) return false;

        this.cp.kill();

        return true;
    }
}

module.exports = Resume;

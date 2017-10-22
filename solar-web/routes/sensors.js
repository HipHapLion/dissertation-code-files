const express = require("express");
const router = express.Router();
const Sensors = require("../models/sensors");
const config = require("../config/database");
const json2csv = require("json2csv");
const fs = require("fs");
const path = require("path");
const bus = require("../eventBus");

// Get data
router.get("/getData", (req, res, next) => {
    Sensors.getData(function (err, data) {
        if (err) {
            res.json({
                msg: "Something went wrong!",
                error: err
            });
        } else {
            res.json(data);
        }
    });
});

// get limited data
router.post("/getLimitData", (req, res, next) => {
    const start = req.body.start;
    const end = req.body.end;

    Sensors.getDataByTimeline(start, end, (err, data) => {
        return res.json(data);
    });
});

// get limited data
router.post("/getLatest", (req, res, next) => {
    const date = new Date(req.body.date);
    // console.log(date);
    Sensors.getLatest(date, (err, data) => {
        if (err) {
            return res.json({ msg: "Something went wrong.", error: err });
        } else {
            return res.json(data);
        }
    });
});

router.post("/exportData", (req, res, next) => {
    var json = req.body.jdata;
    var fields = req.body.fields;
    try {
        var csv = json2csv({ data: json, fields: fields });
        fs.writeFile('export.csv', csv, function (err) {
            if (err) throw err;
            console.log('file saved');
        });
    } catch (err) {
        console.error(err);
    }
    return res.json({ msg: "recieved data" });
});

router.get("/export", (req, res, next) => {
    return res.sendFile(path.join(__dirname, "../export.csv"));
});

// Add data
router.post("/addData", (req, res, next) => {
    let newData = new Sensors({
        date: req.body.date*1000,
        light: req.body.light,
        tt1: req.body.tt1,
        tt2: req.body.tt2,
        tt3: req.body.tt3,
        tt4: req.body.tt4,
        tt10: req.body.tt10,
        ft1: req.body.ft1,
        ft3: req.body.ft3,
        ft5: req.body.ft5
    });
    Sensors.addData(newData, (err, data) => {
        if (err) {
            res.json({
                success: false,
                msg: "Failed to add data",
                error: err
            });
        } else {
            // Trigger the event somewhere else
            bus.emit('eventName');
            
            res.json({
                success: true,
                msg: "Successfully added new data"
            });
        }
    });
});

module.exports = router;
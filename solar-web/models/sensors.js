const mongoose = require("mongoose");
const config = require("../config/database");

// Sensors schema
const SensorsSchema = mongoose.Schema({
    date: {
        type: Date,
        required: true
    },
    light: {
        type: Number
    },
    tt1: {
        type: Number
    },
    tt2: {
        type: Number
    },
    tt3: {
        type: Number
    },
    tt4: {
        type: Number
    },
    tt10: {
        type: Number
    },
    ft1: {
        type: Number
    },
    ft3: {
        type: Number
    },
    ft5: {
        type: Number
    }   
});

const Sensors = module.exports = mongoose.model("sensor", SensorsSchema);

module.exports.getDataById = function (id, callback) {
    SensorData.findById(id, callback);
}

module.exports.getData = function(callback){
    Sensors.find(callback);
}

module.exports.getDataByTimeline = function(start, end, callback){
    const query= { $and: [ { "date": { $gte: new Date(start) } }, { "date": { $lte: new Date(end) } } ] };
    Sensors.find(query, callback);
}

module.exports.getLatest = function(date, callback){
    const query = { "date": { $gte: new Date(date) } };
    Sensors.find(query, callback).sort({ "date":  -1 }).limit(60);
}

module.exports.addData = function(newData, callback){
    newData.save(callback);
}
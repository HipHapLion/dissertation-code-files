const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
var app = express();
const http = require('http').Server(app);
const io = require('socket.io')(http);
const cors = require('cors');
const bus = require("./eventBus");

// local imports
const sensors = require('./routes/sensors');
const config = require('./config/database');

// socket part
io.on('connection', (socket) => {
    console.log('user connected');

    socket.on('disconnect', function () {
        console.log('user disconnected');
    });

    socket.on('add-message', (message) => {
        console.log(message);
        io.emit('message', { type: 'new-message', text: message });
    });

    socket.on('new', (message) => {
        console.log(message);
    });
});

// Register event listener
bus.on('eventName', function () {
    io.emit('message', { type: 'new-message', text: "msg from Dlistner" });
});

// Express config
const port = process.env.PORT || 3000;
app.use(cors());
app.use(bodyParser.json());
app.use("/sensors", sensors);
app.use(express.static(path.join(__dirname, "public")));
// Mongoose config
mongoose.connect(config.database);

// On db connection
mongoose.connection.on("connected", () => {
    console.log("connected to DB " + config.database);
});

// On error
mongoose.connection.on("error", (err) => {
    console.log("connection error: " + err);
});

// API points
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, "public/index.html"));
});

http.listen(port, () => {
    console.log("server started on port: " + port);
});
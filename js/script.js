const socket_address = "localhost:8765";
let socket;
let counterFrontend = 0;
let counterServer = 0;
const counterFrontendElement = document.getElementById("counter-frontend");
const counterServerElement = document.getElementById("counter-server");
const incrementButton = document.getElementById("incr_count");

function getTimestampS(){
    return (performance.now() / 1000).toFixed(3); 
}

// Calculate time between sending message and geting acknowledgement
function calculateRoundTripTimeMS(now, msg_timestamp){
    const diffSec = now - msg_timestamp;
    const diffMSec = diffSec * 1000;
    return diffMSec.toFixed(1);
}

// Create WebSocket and process its events
function addSocket(socket_address){
    socket = new WebSocket(`ws://${socket_address}`);

    socket.onopen = function() {
        console.log('Connect OK!');
        sendMessage(socket, "handshake");
    };
    
    socket.onclose = function(event) {
        console.log('Connection closed!');
    };
      
    socket.onmessage = function(event) {messageProcessing(event)};
    
    socket.onerror = function(error) { console.log("Error " + error.message) };
    return socket;
}

// Process incoming via websocket messages
function messageProcessing(event){
    let message = JSON.parse(event.data);
    let now = getTimestampS();
    console.log("Got msg from backend at "+ now + "s: " + event.data);
    if (message.type === "increment_acknowledgement"){
        console.log("Roundtrip time: " +  calculateRoundTripTimeMS(now, message.timestamp) + "ms");
        counterServer++;
        counterServerElement.textContent = counterServer;
    } else if (message.type === "handshake_acknowledgement"){
        console.log("Handshake acknowledgement is received from server")
    }
}

// Send command to backend
function sendMessage(socket, type_msg){
    let msg = {
        type: type_msg,
        timestamp: getTimestampS(),
    }
    let msg_json = JSON.stringify(msg);
    try {
        socket.send(msg_json);
        console.log("Sent: "+msg_json);
    } catch (e) {
        console.log(e);
    }
}

function incrementCounter(){
    counterFrontend++;
    counterFrontendElement.textContent = counterFrontend;
    sendMessage(socket, "increment");
}

document.addEventListener("DOMContentLoaded", function () {
    socket = addSocket(socket_address);
    incrementButton.addEventListener("click", incrementCounter);
});

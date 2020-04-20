var net = require('net');

function handler(socket) {
    socket.write('Welcome, My Friend ~~\n');
    socket.on('data', function(data) { socket.write(data); });
}

var svr = net.createServer(handler);

svr.listen(1122);

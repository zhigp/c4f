var events = require('events');
var net = require('net');

var ch = new events.EventEmitter();
ch.clients = {};
ch.subs = {};

function hdJoin(id, client) {
    this.clients[id] = client;
    this.subs[id] = function(senderId, msg) {
        if(id != senderId)
            this.clients[id].write(msg);
    };
    this.on('msg', this.subs[id]);
    client.write('Welcome!\n' + 'Guests Online: ' + this.listeners('msg').length + '\n');
}

function hdData(data) {
    data = data.toString();
    var id = this.remoteAddress + ':' + this.remotePort;
    if(data == 'shutdown\r\n')
        ch.emit('shutdown');
    ch.emit('msg', id, data);
}

function hdLeave(id) {
    ch.removeListener('msg', this.subs[id]);
    ch.emit('msg', id, id+' has left the chat.\n');
}

function hdShutdown() {
    ch.emit('msg', '', 'Chat has shut down.\n');
    ch.removeAllListeners('msg');
}

function hdSvrCreate(client) {
    var id = client.remoteAddress + ':' + client.remotePort;
    ch.emit('join', id, client);
    client.on('data', hdData);
    client.on('close', function() { ch.emit('leave', id) });
}

ch.on('join', hdJoin);
ch.on('leave', hdLeave);
ch.on('shutdown', hdShutdown);

var svr = net.createServer(hdSvrCreate);
svr.listen(1122);
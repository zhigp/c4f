var fs = require('fs');
var join = require('path').join;
var http = require('http');
var parse = require('url').parse;

var root = __dirname;

function hdSvrCreate(req, res) {
    var url = parse(req.url);
    var path = join(root, url.pathname);

    fs.stat(path, function(err, stat) {
        if(err) {
            if('ENOENT' == err.code) {
                res.statusCode = 404;
                res.end('Not Found');
            } else {
                res.statusCode = 500;
                res.end('Internal Server Error');
            }
        } else {
            res.setHeader('Content-Length', stat.size);
            var rStream = fs.createReadStream(path);
            rStream.pipe(res);
            rStream.on('error', function(err) {
                res.statusCode = 500;
                res.end('Internal Server Error');
            });
        }
    });
}

var svr = http.createServer(hdSvrCreate).listen(1122);

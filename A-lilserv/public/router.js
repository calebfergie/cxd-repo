/* adapted from https://developer.mozilla.org/en-US/docs/Learn/Server-side/Express_Nodejs/Introduction#Creating_route_handlers*/

/*SETUP*/
var express = require('express')
var path = require('path')
var lilrouter = express.Router()

/*ROUTING INSTRUCTIONS*/
lilrouter.get('/', function(req, res) {
  res.sendFile(path.join(__dirname + '/index.html'));
});

lilrouter.get('/explaination.html', function(req, res) {
  console.log('somebody submitted' + req.url);
  res.sendFile(path.join(__dirname + '/explaination.html'));
});

module.exports = lilrouter;
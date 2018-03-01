/*
Making Things Talk 3rd ed.
a simple web server in node.js. Requires express.js as well.
context: node.js
*/

var express = require('express');
var router = express.Router();

// define the callback function that's called when
// a client makes a request:
function respondToClient(request, response) {
  console.log(request.connection.remoteAddress);
  console.log(request.headers);
  console.log(request.query);
  console.log('sensor val is: ' + request.params.sensorVal);
  // write back to the client:
  response.write("Hello, client!\n");
  response.end();
}

// define what to do when the client requests something:
router.get('/:sensorVal', respondToClient);
module.exports = router;
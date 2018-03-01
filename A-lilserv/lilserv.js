/* modified from http://expressjs.com/en/starter/hello-world.html, http://expressjs.com/en/resources/middleware/serve-static.html*/
/* thx to mozilla tutorials as well: https://developer.mozilla.org/en-US/docs/Learn/Server-side/Express_Nodejs/routes

/* SETUP */
var express = require('express')
var router = require('./public/router.js'); /*linking to a a router file*/
var lilserv = express()

/*EXECUTION*/
lilserv.use('/', router) /*refer to the router when requests are made*/

lilserv.listen(8080, function() {
  console.log('launched on 8080 baby') /*log the initation of the server*/
})
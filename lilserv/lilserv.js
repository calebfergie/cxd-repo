/* modified from http://expressjs.com/en/starter/hello-world.html, http://expressjs.com/en/resources/middleware/serve-static.html*/
/* thx to mozilla tutorial

/* SETUP */
var express = require('express')
var router = require('./public/router.js'); /*linking to a a router file*/
var lilserv = express()

/*EXECUTION*/
lilserv.use('/', router) /*refer to the router when requests are made*/

lilserv.listen(8080, function() {
  console.log('launched on 8080 baby') /*log the initation of the server*/
})


/* static fun, using .use
var favicon = require('serve-favicon') ----- module not included
var path = require('path')
var favicon = require('serve-favicon') ----- module not included
var lilstatic = require('serve-static')
lilserv.use(favicon(path.join(__dirname, 'public', 'favicon.ico'))) ----- module not included add favicon when page requested
 lilserv.use(lilstatic(path.join(__dirname, 'public'))) */
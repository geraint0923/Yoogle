
/**
 * Module dependencies.
 */

var express = require('express')
  , routes = require('./routes')
  , search = require('./routes/search')
  , user = require('./routes/user')
  , ydb = require('ydb')
  , http = require('http')
  , fs = require('fs')
  , path = require('path');

var app = express();

function readLines(filename, func) {
	var remaining = '';
	remaining += fs.readFileSync(filename);
	var index = remaining.indexOf('\n');
	while (index > -1) {
		var line = remaining.substring(0, index);
		remaining = remaining.substring(index + 1);
		func(line);
		index = remaining.indexOf('\n');
	}
	if (remaining.length > 0) {
		func(remaining);
	}
}


function func(data) {
	  var record = JSON.parse(data);
//	  console.log(record["name"]);
	  ydb.insert_record(record["_id"], 
			  record["addr"],
			  record["fetdt"]["$date"],
			  record["latlng"][0],
			  record["latlng"][1],
			  record["name"],
			  record["pcode"],
			  record["url"]);
}

readLines('data.json', func);


app.configure(function(){
  app.set('port', process.env.PORT || 3000);
  app.set('views', __dirname + '/views');
  app.set('view engine', 'ejs');
  app.use(express.favicon());
  app.use(express.logger('dev'));
  app.use(express.bodyParser());
  app.use(express.methodOverride());
  app.use(app.router);
  app.use(express.static(path.join(__dirname, 'public')));
  app.use(express.favicon(__dirname + '/public/images/favicon.ico'));
});

app.configure('development', function(){
  app.use(express.errorHandler());
});

app.get('/', routes.index);
app.get('/users', user.list);

app.post('/list_search', search.list_search);
app.post('/set_position', search.set_position);

http.createServer(app).listen(app.get('port'), function(){
  console.log("Express server listening on port " + app.get('port'));
});

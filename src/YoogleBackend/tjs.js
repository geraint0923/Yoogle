var fs = require('fs');
var ydb = require('./build/Release/ydb');

function readLines(input, func) {
	var remaining = '';

    input.on('data', function(data) {
		remaining += data;
		var index = remaining.indexOf('\n');
		while (index > -1) {
			var line = remaining.substring(0, index);
			remaining = remaining.substring(index + 1);
			func(line);
			index = remaining.indexOf('\n');
		}
	});

	input.on('end', function() {
		if (remaining.length > 0) {
			func(remaining);
		}
	});
}

function func(data) {
	  var record = JSON.parse(data);
	  console.log(record["name"]);
	  ydb.insert_record(record["_id"], 
			  record["addr"],
			  record["fetdt"]["$date"],
			  record["latlng"][0],
			  record["latlng"][1],
			  record["name"],
			  record["pcode"],
			  record["url"]);
}

var input = fs.createReadStream('data.json');
readLines(input, func);

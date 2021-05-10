var path = require('path');
var util = require("util");
var bindingPath = path.join(__dirname, "binding", util.format("napi-v3-%s-%s/node_sqlite3.node", process.platform, process.arch));
var binding = require(bindingPath);

module.exports = exports = binding;

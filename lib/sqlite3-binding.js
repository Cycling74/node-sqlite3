var path = require('path');
var util = require("util");

var binding;
switch (process.platform) {

case "darwin":
    if (process.arch === "x64") {
        binding = require("./binding/napi-v3-darwin-x64/node_sqlite3.node");
    } else {
        binding = require("./binding/napi-v3-darwin-arm64/node_sqlite3.node");
    }
    break;
case "win32":
    binding = require("./binding/napi-v3-win32-x64/node_sqlite3.node");
    break;
default:
    binding = require(path.join(__dirname, "binding", util.format("napi-v3-%s-%s/node_sqlite3.node", process.platform, process.arch)));

}

module.exports = exports = binding;

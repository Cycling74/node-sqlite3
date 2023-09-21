const util = require("util");
const { join } = require("path");

let binding;
switch (process.platform) {

case "darwin":
    if (process.arch === "x64") {
        binding = require("./binding/napi-v6-darwin-x64/node_sqlite3.node");
    } else {
        binding = require("./binding/napi-v6-darwin-arm64/node_sqlite3.node");
    }
    break;
case "win32":
    binding = require("./binding/napi-v6-win32-x64/node_sqlite3.node");
    break;
default:
    binding = require(join(__dirname, "binding", util.format("napi-v6-%s-%s/node_sqlite3.node", process.platform, process.arch)));

}

module.exports = exports = binding;
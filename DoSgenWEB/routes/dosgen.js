//Importácia modulov
var express = require('express');
var router = express.Router();
var spawn = require('child_process').spawn;
var process = require('process');
var io = require("socket.io");
var logout = require('express-passport-logout');
var dosgenProc = false;
module.exports = router;

//Funkcia po obdržaní HTTP GET "/"
router.get('/', function (req, res, next) {
    res.render('dosgen', {});
});

//Funkcia po obdržaní HTTP GET "/logout"
router.get('/logout', function (req, res) {
    req.logout();
    res.redirect('/');
});

//Funkcia po obdržaní HTTP POST "/fire", zahájenie útoku
router.post("/fire", function (req, res, next) {
    var args = parseAttacksArguments(req.body);
    /* Spustenie nástroja DoSgen: <cesta k binárnemu súboru>,
    <zozbierané argumenty s hodnotami> */
    dosgenProc = spawn("/home/user/app/DoSgen/dosgen", args, {detached: true});
    //Štandardný výstup
    dosgenProc.stdout.on('data', function (data) {
        io.sockets.emit("stdoutData", data.toString());
    });
    //Štandardný error
    dosgenProc.stderr.on('data', function (data) {
        io.sockets.emit("stderrData", data.toString());
    });
    //Akcia po ukončení procesu
    dosgenProc.on("exit", function () {
        io.sockets.emit("closedProc", false);
        dosgenProc = false;
    });
    //Error pri neúspešnom spustení
    dosgenProc.on('error', function (error) {
        var resJson = {
            err: error
        };
        console.log(resJson);
    });
    res.send({
        err: false
    });
});
//Funkcia po obdržaní HTTP POST "/kill", zabitie procesu útoku
router.post("/kill", function (req, res, next) {
    if (dosgenProc != false) {
        process.kill(-dosgenProc.pid);
        dosgenProc = false;
    }
});
//Parsovanie argumentov
function parseAttacksArguments(arguments) {
    var args;
    var attacksSettings = {};

    for (var attackFullName in arguments) {
        if (arguments.hasOwnProperty(attackFullName)) {
            var attackValue = arguments[attackFullName];
            var attackTypeAndArg = attackFullName.split(":");
            if (attackTypeAndArg.length != 2)
                continue;
            var attackType = attackTypeAndArg[0];
            var attackArg = attackTypeAndArg[1];
            if (attacksSettings[attackType] === undefined) {
                attacksSettings[attackType] = {};
            }
            attacksSettings[attackType][attackArg] = attackValue;
        }
    }
    //General Settings
    var generalSettings = attacksSettings["gen"];
    args = [];
    for (var generalSettingArg in generalSettings) {
        if (generalSettings.hasOwnProperty(generalSettingArg)) {
            var argGen = generalSettings[generalSettingArg];
            if (argGen == "") {
                continue;
            }
            args.push("-" + generalSettingArg);
            args.push(argGen);
        }
    }
    delete attacksSettings["gen"];
    //Jednotlivé útoky
    for (var attackSettingsKey in attacksSettings) {
        if (attacksSettings.hasOwnProperty(attackSettingsKey)) {
            var attackSettings = attacksSettings[attackSettingsKey];
            args.push("--" + attackSettingsKey);
            for (var attackSettingsArg in attackSettings) {
                if (attackSettings.hasOwnProperty(attackSettingsArg)) {
                    var arg = attackSettings[attackSettingsArg];
                    if (arg == "") {
                        continue;
                    }
                    args.push("-" + attackSettingsArg);
                    args.push(arg);
                }
            }
        }
    }
    //Výsledné pole reťazcov argumentov
    return args;
}

//IO sockety
module.exports.startSocketIo = function (server) {
    io = io.listen(server);
    io.sockets.on("connection", function (client) {
        /*V prípade zmeny stavu spojenia (obnovenie, opustenie stránky,
        strata konektivity) bude prípadne bežiaci proces zabitý*/
        client.on("disconnect", function () {
            if (dosgenProc != false) {
                process.kill(-dosgenProc.pid);
                dosgenProc = false;
            }
        });
    });
};

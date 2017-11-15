//Importácia modulov
var express = require('express');
var http = require('http');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var expressSession = require('express-session');
var fs = require("fs");
var https = require('https');
var dosgen = require("./routes/dosgen");
var app = express();
var configFileName = "config.json";
var config = JSON.parse(fs.readFileSync(path.join(__dirname, configFileName)));
GLOBAL.config = config;
var passport = require(path.join(__dirname, '/passport.js'));
var auth = require(path.join(__dirname, '/routes/auth'))(passport);

//Nastavenie naslúchajúceho portu
app.set("port", 8888);
//Cesta k HTML súborom
app.set("views", path.join(__dirname, "views"));
//Nastavenie zobrazovacieho systému na EJS
app.set("view engine", "ejs");
//Logger
app.use(logger("dev"));
//Počítanie HMAC na základe kľúča
app.use(expressSession({secret: 'mySecretKey', saveUninitialized: true, resave: true}));
//Parsovací middleware
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
//Cookie parser
app.use(cookieParser());
//Obsluha statických súborov (obrázky, .js, .css, ...)
app.use(express.static(path.join(__dirname, 'public'), {maxAge: 86400000 }));
//Autentizačný middleware
app.use(passport.initialize());
app.use(passport.session());
app.use('/auth', auth);
app.use(passport.requireAuth);
//Využitie modulu dosgen pre HTTP GET "/" a "/dosgen"
app.use("/dosgen", dosgen);
app.use("/", dosgen);
//Stránka nenájdená
app.use(function(req, res, next) {
    var err = new Error("Error, not found!");
    err.status = 404;
    next(err);
});
//Súkromný kľúč a certifikát
var options = {
    key: fs.readFileSync('key.pem'),
    cert: fs.readFileSync('cert.pem')
};
//Spustenie HTTPS serveru
var httpsServer = https.createServer(options, app).listen(app.get("port"), function(){
    console.log("Server is now running...");
});
//Spustenie IO socketov pre HTTPS server
dosgen.startSocketIo(httpsServer);
var passport = require("passport");
var LocalStrategy = require("passport-local").Strategy;
var adminUsername = global.config.username;
var adminPassword = global.config.password;
//Lokálne prihlasovanie
passport.use(new LocalStrategy({
        usernameField: "username",
        passwordField: "password"
    },
    function (username, password, done) {
        //Úspešná autentizácia
        if ((username == adminUsername) && (password == adminPassword)) {
            console.log("Authentication successful for: " + username);
            return done(null, {username: username});
        }
        //Neúspešná autentizácia
        else {
            console.log("Authentication failed for : " + username);
            return done(null, false);
        }
    }
));
//Serializácia
passport.serializeUser(function (user, done) {
    done(null, user.username);
});
//Deserializácia
passport.deserializeUser(function (username, done) {
    done(null, {username: username})
});

passport.requireAuth = function (req, res, next) {
    //Presmerovanie v prípade neúspešnej autentizácie
    if (!req.isAuthenticated()) {
        res.redirect("/auth/login");
    }
    /*Pokračovanie k ďalšiemu modulu (v server.js)
     v prípade úspešnej autentizácie*/
    else {
        next();
    }
};
module.exports = passport;
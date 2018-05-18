module.exports = function(passport) {
  var express = require("express");
  var router = express.Router();
  //Presmerovanie k prihlasovaciemu formuláru
  router.get("/", function(req, res, next) {
    res.redirect("/auth/login/");
  });
  //Presmerovanie pri úspešnom/neúspešnom prihlásení
  router.post("/login", passport.authenticate("local", {
    failureRedirect: "/auth/login?status=fail",
    successRedirect: "/dosgen"
  }));
  //Akcia pri neúspešnom prihlásení
  router.get("/login", function(req, res, next) {
    var status = req.query.status;
    res.render('login', {
      failedLogin: status == "fail"
    });
  });
  return router;
};

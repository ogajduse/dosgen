//Príznak zabitia procesu
var isKilled = true;
//Buffer pre výstup
var buffer = "";
//Hviezdička označujúca povinné vstupné pole
var asterisk = '<span style="color:red; font-size: 18px">*</span>';
//Počítadlo pridaných útokov
var attackCounter = 0;
//Nastavenia útokov
var attacks = [{
    "name": "SYN Flood", //Meno (titulok)
    "id": "syn", //ID
    "fields": [
      //ID vstupného poľa, jeho názov, placeholder, (hviezdička)
      ["s", 'Source IP address', "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["S", "Source port", "Empty means random"],
      ["D", "Destination port", "Empty means random"],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "RST Flood",
    "id": "rst",
    "fields": [
      ["s", 'Source IP address', "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["S", "Source port", "Empty means random"],
      ["D", "Destination port", "Empty means random"],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "UDP Flood",
    "id": "udp",
    "fields": [
      ["s", "Source IP address", "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["S", "Source port", "Empty means random"],
      ["D", "Destination port", "Empty means random"],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "ICMP Flood",
    "id": "icmp",
    "fields": [
      ["s", 'Source IP address', "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "ARP Flood",
    "id": "arp",
    "fields": [
      ["s", "Source IP address", "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "DNS Flood",
    "id": "dns",
    "fields": [
      ["s", "Source IP address", "Empty means random"],
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["S", "Source port", "Empty means random"],
      ["n", "DNS name", "E.g. www.vutbr.cz", asterisk],
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "DHCP Starvation",
    "id": "dhcp",
    "fields": [
      ["p", "Payload [B]", "Empty means 0 bytes"]
    ]
  },
  {
    "name": "HTTP flood",
    "id": "http",
    "fields": [
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["D", "Destination port", "Empty means port 80"],
      ["p", "Path", "Empty means '/' index"],
      ["v", "Number of threads", "Empty means 1"]
    ]
  },
  {
    "name": "Slowloris",
    "id": "slowLoris",
    "fields": [
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["D", "Destination port", "Empty means port 80"],
      ["t", "Timeout", "Empty means 15 s"],
      ["c", "Number of connections", "Empty means 300"]
    ]
  },
  {
    "name": "R.U. Dead Yet?", //Meno (titulok)
    "id": "rudy", //ID
    "fields": [
      //ID vstupného poľa, jeho názov, placeholder, (hviezdička)
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["D", "Destination port", "Empty means port 80"],
      ["t", "Timeout", "Empty means 15 s"],
      ["c", "Number of connections", "Empty means 300"],
      ["p", "Requested form", ""],
      ["P", "Parameter", "Form data"]
    ]
  },
  {
    "name": "Slow Read",
    "id": "slowRead",
    "fields": [
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["D", "Destination port", "Empty means 80"],
      ["c", "Number of connections", "Empty means 200"],
      ["p", "Path", "Empty means '/' index"],
      ["w", "Window size", "Empty means 50"],
      ["b", "Buffer size [B]", "Empty means 5"],
      ["n", "Read interval [ms]", "Empty means 1000"]

    ]
  },
  {
    "name": "SSH dictionary attack", //Meno (titulok)
    "id": "ssh", //ID
    "fields": [
      ["d", "Destination IP address", "Specify your target", asterisk],
      ["p", "Port", "Empty means port 22"],
      ["u", "User", , asterisk],
      ["l", "Dictionary", "Enter full path", asterisk]
    ]
  },
  {
    "name": "NTP Flood ", //Meno (titulok)
    "id": "ntp", //ID
    "fields": [
      ["s", "Source IP address", "Specify your target", asterisk],
      ["d", "Destination IP address", "Specify NTP server", asterisk],
      ["p", "Port", "Empty means port random"],
    ]
  },
  {
    "name": "SNMP Flood ", //Meno (titulok)
    "id": "snmp", //ID
    "fields": [
      ["s", "Source IP address", "Specify your target", asterisk],
      ["d", "Destination IP address", "Specify SNMP agent to query", asterisk],
      ["p", "Port", "Empty means random port"],
    ]
  },
  {
    "name": "SSDP Flood ", //Meno (titulok)
    "id": "snmp", //ID
    "fields": [
      ["s", "Source IP address", "Specify your target", asterisk],
      ["d", "Destination IP address", "Specify destination address", asterisk],
      ["p", "Port", "Empty means random port"],
    ]
  },
];

$(function() {
  //Upozornenie obnovenia/opustenia stránky (vypnuté)
  $(window).off("beforeunload");
  //Tlačidlo spustenia útoku (vypnuté)
  $('#runButton').prop('disabled', true);
  //Tlačidlo ukončenia útoku (vypnuté)
  $('#stopButton').prop('disabled', true);
  //Potvrdenie odhlásenia pomocou Bootbox
  $('.signout').click(function() {
    bootbox.confirm({
      size: 'small',
      title: "Leaving...",
      message: "Are you sure to sign out from DoSgen?",
      buttons: {
        confirm: {
          label: "Sign out!"
        }
      },
      //Odhlásenie
      callback: function(result) {
        if (result === true) {
          window.location.href = 'logout';
        }
      }
    })
  });

  //Inicializácia Masonry
  $('#container').masonry({
    itemSelector: '.item',
    columnWidth: 70
  });
  //Formulár pre spustenie útoku
  $("#fireForm").submit(function(e) {
    $.ajax({
      type: "POST",
      url: "/dosgen/fire",
      data: $("#fireForm").serialize()
    });
    e.preventDefault();
    $('#runButton').prop('disabled', true);
    $('#stopButton').prop('disabled', false);
    //Dĺžka útoku
    var time = document.getElementById('time').value * 1000;
    if (time != "0") {
      setTimeout(stopAttack, time);
    }
    //Vyčistenie výstupu + bufferu
    $("#output").val("");
    buffer = "";
    isKilled = false;
    //Správa zobrazená pri obnovení/opustení stránky počas bežiaceho útoku
    $(window).on("beforeunload", function(e) {
      var message = "If you reload or leave this page during attacking, attack will be stopped.";
      e.returnValue = message;
      return message;
    });
  });
  //Ukončenie útoku
  $("#killForm").submit(function(e) {
    $.ajax({
      type: "POST",
      url: "/dosgen/kill",
      data: $("#killForm").serialize()
    });
    isKilled = true;
    e.preventDefault();
    $(window).off("beforeunload");
  });

  //Funkcia ukončenia útoku zavolaná po zadaní dĺžky útoku
  function stopAttack() {
    $.ajax({
      type: "POST",
      url: "/dosgen/kill",
      data: $("#killForm").serialize()
    });
    isKilled = true;
    $(window).off("beforeunload");
  }

  //Tlačidlo pridania útoku
  $('#addAttackBtn').click(function() {
    addAttackCallback();
  });
  //Tlačidlo odobrania útoku
  $(document).on("click", '#removeAttackBtn', function() {
    var attackId = $(this).closest("div.item").attr("id");
    removeElementFromMasonry(attackId);
  });
  //Zostavenie výberového zoznamu útokov
  var attackSelectBox = $('#attackSelectBox');
  attacks.forEach(function(attack) {
    var attackId = attack["id"];
    var attackName = attack["name"];
    attackSelectBox.append($("<option/>", {
      value: attackId,
      text: attackName
    }));
  });
  //Inicializácia funkcie výberového zoznamu útokov
  $('.selectpicker').selectpicker({
    style: 'btn-select',
    size: 8,
    width: 150
  });
  setupSocketIo();
});
//Pridanie vybraného útoku, následne odobratie zo zoznamu
function addAttackCallback() {
  var selectedAttack = $('#attackSelectBox').val();
  attacks.forEach(function(attack) {
    if (attack["id"] == selectedAttack) {
      addAttackSettings(attack);
      $('#attackSelectBox option:selected').remove();
      $('.selectpicker').selectpicker('refresh');
    }
  });
}
//Pridanie nastavení útokov, generácia HTML bloku
function addAttackSettings(attackSettingsProps) {
  var attackName = attackSettingsProps["name"];
  var attackId = attackSettingsProps["id"];
  var fields = attackSettingsProps["fields"];
  //Validácia vstupnej hodnoty pre IP adresy
  var patternIP = 'pattern="^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.)' +
    '{3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"';
  var el = '<div id="' + attackId + '" class="item">';
  el += '<span id="removeAttackBtn" class="glyphicon glyphicon-remove-sign"></span>';
  el += '<div class="attack-title">' + attackName + '</div>';
  el += '<div class="form-group">';


  fields.forEach(function(field) {

    var fieldText = field[1];
    var fieldName = attackId + ":" + field[0];
    //Pokiaľ nie je definovaná povinnosť poľa, neuvádzaj "required"
    if (field[3] === undefined) {
      el += '<label for="' + fieldName + '">' + fieldText + ':</label>';
      //Pokiaľ sa jedná o IP adresy, validuj!
      if (field[0] === "s" || field[0] === "d") {
        el += '<input type="text" class="form-control" ' + patternIP + ' name="' +
          fieldName + '" placeholder="' + field[2] + '">';
      } else {
        el += '<input type="text" class="form-control" name="' +
          fieldName + '" placeholder="' + field[2] + '">';
      }
    } else {
      el += '<label for="' + fieldName + '">' + fieldText + ': ' + field[3] + '</label>';
      //Pokiaľ sa jedná o IP adresy, validuj!
      if (field[0] === "s" || field[0] === "d") {
        el += '<input type="text" class="form-control"' + patternIP + ' name="' +
          fieldName + '" placeholder="' + field[2] + '" required>';
      } else {
        el += '<input type="text" class="form-control" name="' + fieldName + '" placeholder="' +
          field[2] + '" required>';
      }
    }
  });
  el += '</div></div>'
  addElementToMasonry(el);
  // console.log(el);
}
//Pridanie HTML bloku do Masonry
function addElementToMasonry(el) {
  var container = $('#container');
  container.append(el);
  container.masonry("reloadItems");
  container.masonry("layout");
  attackCounter++;
  if (attackCounter != 0) {
    $('#runButton').prop('disabled', false);
  }
}
//Celkové odobratie HTML bloku
function removeElementFromMasonry(attackIdToRemove) {
  var attackWindow = $('#' + attackIdToRemove);
  attackWindow.remove();
  var container = $('#container');
  container.masonry("reloadItems");
  container.masonry("layout");
  var attackSelectBox = $('#attackSelectBox');

  attacks.forEach(function(attack) {
    var attackId = attack["id"];
    var attackName = attack["name"];

    if (attackId != attackIdToRemove)
      return;

    attackSelectBox.append($("<option/>", {
      value: attackId,
      text: attackName
    }));
    $('.selectpicker').selectpicker('refresh');
    attackCounter--;

    if (attackCounter == 0) {
      $('#runButton').prop('disabled', true);
    }
  });
}


//Pridanie dát na výstup
function addToOutput(data) {
  if (isKilled == false) {
    var textbox = $("#output");
    textbox.val(textbox.val() + data);
  } else {
    buffer += data;
  }
}

document.getElementById("textbox").scrollTop = document.getElementById("textbox").scrollHeight;

//Počítanie rýchlostí útoku pomocou regulárných výrazov
function closeHandler() {
  $('#runButton').prop('disabled', false);
  $('#stopButton').prop('disabled', true);
  $(window).off("beforeunload");

  var textbox = $("#output");
  var rePacket = /(\d+) packets outgoing/;
  var reByte = /(\d+) bytes outgoing/;
  var reTime = /(\d+) sec, (\d+) usec on CPU(\d+) \((\d+) packets\)/g;
  var str = buffer;
  var matchPacket;
  var matchByte;
  var matchTime;
  var usecCounter = 0;
  var secCounter = 0;
  var i = 0;

  //Zozbieranie hodnôt času z výstupu
  while (matchTime = reTime.exec(str)) {
    usecCounter += parseInt(matchTime[2]);
    secCounter += parseInt(matchTime[1]);
    i++;
  }
  //Zozbieranie počtu prenesených paketov a bajtov
  if ((matchPacket = rePacket.exec(str)) && (matchByte = reByte.exec(str)) !== null) {
    if (matchPacket.index === rePacket.lastIndex && matchByte.index === reByte.lastIndex) {
      rePacket.lastIndex++;
      reByte.lastIndex++;
    }
    //Výpočet pps a vyťaženia linky
    var attackTime = (secCounter / i) + ((usecCounter / i) / 1000000);
    var power_pps = Math.round((matchPacket[1] / attackTime));
    var power_mb = ((matchByte[1] / attackTime) / 1000000).toFixed(2);
    var rate = "DoS attack with approx. rate: " + power_pps + " pps,\ " +
      "link utilization: " + power_mb + " MB/s.\n\nDetailed info:";
    //Vypísanie správneho výstupu s výpočtom
    textbox.val(textbox.val() + "\n" + rate + "\n" + buffer);
  }
  //V prípade chybného výpisu nenastane počítanie, vypíše sa daná chyba bez výpočtu
  else {
    textbox.val(textbox.val() + "\n" + buffer);
  }
}
//Zostavenie IO socketu
function setupSocketIo() {
  var socket = io.connect();
  socket.on("stdoutData", function(data) {
    addToOutput(data);
  });
  socket.on("stderrData", function(data) {
    addToOutput(data);
  });
  socket.on("closedProc", closeHandler);
}

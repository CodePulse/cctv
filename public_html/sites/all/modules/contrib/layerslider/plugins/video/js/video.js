(function() {
  data = new Object();

  function get(id) {
    return document.getElementById(id);
  }

  function getVal(id) {
    var elm = get(id);

    if (elm.nodeName == "SELECT")
      return elm.options[elm.selectedIndex].value;

    if (elm.type == "checkbox")
      return elm.checked;

    return elm.value;
  }

  function setVal(id, value, name) {
    if (typeof(value) != 'undefined' && value != null) {
      var elm = get(id);

      if (elm.nodeName == "SELECT")
        selectByValue(document.forms[0], id, value);
      else if (elm.type == "checkbox") {
        if (typeof(value) == 'string') {
          value = value.toLowerCase();
          value = (!name && value === 'true') || (name && value === name.toLowerCase());
        }
        elm.checked = !!value;
      } else
          elm.value = value;
    }
  }


  window.Video = {

    formToData : function(field) {
      if (field == "width" || field == "height") {
        this.changeSize(field);
      }
      if (field == "src") {
        this.moveStates(field);
      }
      this.previewVideo();
    },

    beforeResize : function() {
      this.width = parseInt(getVal('width') || 320, 10);
      this.height = parseInt(getVal('height') || 240, 10);
    },

    changeSize : function(type) {
      var width, height;

      if (get('constrain').checked) {
        width = parseInt(getVal('width') || 320, 10);
        height = parseInt(getVal('height') || 240, 10);

        if (type == 'width') {
          this.height = Math.round((width / this.width) * height);
          setVal('height', this.height);
	    } else {
	      this.width = Math.round((height / this.height) * width);
          setVal('width', this.width);
        }
      }
    },

    insertVideo : function() {
      var videodata = get('prev').innerHTML;
      if(!videodata){
        alert('Enter Correct URL');
      }
      else {
        var appFields;
        var doc = window.opener;
        var index = location.href.lastIndexOf('app=');
        if (index == -1) return;
        var data = decodeURIComponent(location.href.substr(index + 4)).split('|');
        var arr, prop, str, appName = data.shift();
        // Extract fields
        for (var i = 0, len = data.length; i < len; i++) {
          str = data[i];
          if (!str.length) continue;
          if (str.indexOf('&') != -1) str = str.split('&')[0];
          arr = str.split('@');
          if (arr.length > 1) {
            prop = arr.shift();
            appFields = arr.join('@');
          }
        }

        doc.eval(appFields)(videodata); // send videodata at parent window function
        this.close();
        doc.focus();
      }
    },

    previewVideo : function() {
      get('prev').innerHTML = this.dataToHtml();
    },

    moveStates : function(field) {
      src = get(field).value;

      if (!src) {
        return;
      }
      else {
        // YouTube Embed
        if (src.match(/youtube\.com\/embed\/\w+/)) {
          data.width = 425;
          data.height = 350;
          data.type = 'iframe';
          setVal('src', src);
        } else {
          // YouTube *NEW*
          if (src.match(/youtu\.be\/[a-z1-9.-_]+/)) {
            data.width = 425;
            data.height = 350;
            data.type = 'iframe';
            src = 'http://www.youtube.com/embed/' + src.match(/youtu.be\/([a-z1-9.-_]+)/)[1];
            setVal('src', src);
          }
          // YouTube
          if (src.match(/youtube\.com(.+)v=([^&]+)/)) {
            data.width = 425;
            data.height = 350;
            data.type = 'iframe';
            src = 'http://www.youtube.com/embed/' + src.match(/v=([^&]+)/)[1];
            setVal('src', src);
          }
        }

        // Google video
        if (src.match(/video\.google\.com(.+)docid=([^&]+)/)) {
          data.width = 425;
          data.height = 326;
          data.type = 'flash';
          src = 'http://video.google.com/googleplayer.swf?docId=' + src.match(/docid=([^&]+)/)[1] + '&hl=en';
          setVal('src', src);
        }

        // Vimeo
        if (src.match(/vimeo\.com\/([0-9]+)/)) {
          data.width = 425;
          data.height = 350;
          data.type = 'iframe';
          src = 'http://player.vimeo.com/video/' + src.match(/vimeo.com\/([0-9]+)/)[1];
          setVal('src', src);
        }

        // stream.cz
        if (src.match(/stream\.cz\/((?!object).)*\/([0-9]+)/)) {
          data.width = 425;
          data.height = 350;
          data.type = 'iframe';
          src = 'http://www.stream.cz/object/' + src.match(/stream.cz\/[^/]+\/([0-9]+)/)[1];
          setVal('src', src);
        }

        // Google maps
        if (src.match(/maps\.google\.([a-z]{2,3})\/maps\/(.+)msid=(.+)/)) {
          data.width = 425;
          data.height = 350;
          data.type = 'iframe';
          src = 'http://maps.google.com/maps/ms?msid=' + src.match(/msid=(.+)/)[1] + "&output=embed";
          setVal('src', src);
        }

        // Set default size
        setVal('width', data.width || 320);
        setVal('height', data.height || 240);
      }
    },

    dataToHtml : function() {
      if(data.type == 'iframe') {
        return '<iframe src="' + get('src').value + '" frameborder="0" allowfullscreen="" height="' + get('height').value + '" width="' + get('width').value + '"></iframe>';
      }
      else {
	    return '';
      }
    },

    close : function() {
      window.close();
    },
  };

})();

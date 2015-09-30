(function() {

  function get(id) {
    return document.getElementById(id);
  }

  function getBrowserHTML(id, target_form_element) {
	var basePath, imceinstalled, html;

	basePath = window.opener.Drupal.settings.layerslider.basePath;
	imceinstalled = window.opener.Drupal.settings.layerslider.imceinstalled;

	if (!imceinstalled)  // check imce installed
		return "";

	html = "";
	html += '<a href="'+ basePath + 'imce?app=layerslider|url@' + target_form_element + '" onclick="window.open(this.href, \'\', \'width=760,height=560,resizable=1\');return false;" class="browse">';
	html += '<span id="' + id + '" title="Browse">Browse</span></a>';

	return html;
  }

  window.Image = {

    init : function() {
      // Setup browse button
      get('srcbrowsercontainer').innerHTML = getBrowserHTML('srcbrowser','src');
    },

    insertImage : function() {
      var src = get('src').value;
      var exts = 'png,gif,jpg,jpeg';
    
      if(!src){
        alert('Enter image Url');
      }
      // Validate extension
      else if((',' + exts + ',').indexOf(',' + src.substr(src.lastIndexOf('.') + 1).toLowerCase() + ',') < 0) {
        return alert('Only files with the following extensions are allowed: ' + exts);
      }
      else {
        var Imagedata = get('prev').innerHTML;
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

        if(prop == 'sendto'){
          doc.eval(appFields)(Imagedata); // send Imagedata at parent window function
        }
        else if(prop == 'url') {
          doc.document.getElementById(appFields).value = src; // send Imagedata at parent window input box
        }
        else {
          return;
        }
        doc.focus();
        this.close();
      }
    },

    previewImage : function(field) {
      var src = get(field).value;
      if (!src) {
        return;
      }
      else {
        get('prev').innerHTML = '<img src="' + get('src').value + '"  />';
      }
    },

    close : function() {
      window.close();
    },
  };

})();

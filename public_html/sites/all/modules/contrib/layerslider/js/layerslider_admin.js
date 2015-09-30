 // JavaScript Document
(function ($) {
  var layerslider   = '.layerslider';
  var ls_window     = '.ls-layer';
  var ls_list_box   = '#edit-layerslider-slide-layer .fieldset-wrapper';

  var ls_add_text   = '#edit-text';
  var ls_add_image  = '#edit-image';
  var ls_add_video  = '#edit-video';
  var ls_edit_css   = '#edit-css-file';

  var ls_text_box   = '#edit-text-html';
  var ls_top_pos    = '#edit-position-top';
  var ls_left_pos   = '#edit-position-left';
  var ls_class_list = '#edit-class-list';
  
  var ls_animation_preview = '#edit-layerslider-slide-animation-preview';

  var slide_setting_box = '#edit-layerslider-slide-options';
  slide_bg_setting_box  = new Array('#edit-background-color', '#edit-background-image', '#edit-background-repeat', '#edit-background-position');
  slide_effect_setting_box = new Array('#edit-slidedirection', '#edit-slidedelay', '#edit-durationin', '#edit-easingin', '#edit-delayin', '#edit-durationout', '#edit-easingout', '#edit-delayout');
  layer_effect_setting_box = new Array('#edit-slidedirection--2', '#edit-durationin--2', '#edit-easingin--2', '#edit-delayin--2', '#edit-slideoutdirection', '#edit-durationout--2', '#edit-easingout--2', '#edit-delayout--2');


  Drupal.behaviors.layerslider = {
    attach: function (context, settings) {

      basePath = Drupal.settings.layerslider.basePath;
      cssfile = Drupal.settings.layerslider.cssfile;
      cssfileBrowse = Drupal.settings.layerslider.cssfileBrowse;
      layersliderPath = Drupal.settings.layerslider.layersliderPath;

      // add slide when slide not added
      if ( $(layerslider).children().length == 0 ) {
        var element = $('<div></div>')[0];
        element.setAttribute('class',ls_window.slice( 1 ));  // set class
        $(layerslider).append(element); // add slide
      }
      $('head').append('<link type="text/css" href="'+cssfile+'" rel="stylesheet" id="layerslider_slide_css"/>');

      // set background value from slidebg box to slide
      Drupal.set_slide_bg(slide_setting_box, slide_bg_setting_box);

      // get background value back to slidebg box
      Drupal.get_slide_bg(slide_setting_box, slide_bg_setting_box);
 
      // remove slide background
      $('.remove', context).live('click', function() {
        Drupal.layerslider_slide_css_attributes_remove(this);
        return false;
      });

      // Effect setting for slide
      $(slide_effect_setting_box, context).each(function () {
        $(slide_setting_box).find(this).live('change keyup', function(event) {
          var slide = $(layerslider).find(ls_window);
          Drupal.layerslider_slide_layer_effects(slide, this, event);
        });
      });

      // get effects value back to slide effect box
      Drupal.get_slide_effects(slide_setting_box, slide_effect_setting_box);

  /* ---------- end of slide global settings  ------------- */


      // binding event first time with all the layers
      $(ls_window, context).children().each(function () {
        $(this).mouseup(function () {
          Drupal.layerslider_slide_layer_selected($(ls_window), $(this).attr( 'id' )); // get the properties of layer on mouse select
        });
        $(this).draggable({
          containment: ls_window,
          cursor: "move",
        });
        if($(this).hasClass( 'selected' )) {
          Drupal.layerslider_slide_layer_selected($(ls_window), $(this).attr( 'id' )); // get the properties of selected layer
        }
      });

      // list of all the layer at list box
      Drupal.layerslider_slide_layer_list(ls_window, ls_list_box);

      // add text layer
      $(ls_add_text, context).click(function () {
        Drupal.layerslider_slide_layer_add(ls_window, "text", "Enter Text");
        return false;
      });
      // add image layer
      $(ls_add_image, context).click(function () {
        window.open(basePath+layersliderPath+'/plugins/image/image.htm?app=layerslider|sendto@Drupal.layerslider_image_path', '', 'width=480,height=500,resizable=1');
        return false;
      });
      // add video layer
      $(ls_add_video, context).click(function () {
        window.open(basePath+layersliderPath+'/plugins/video/video.htm?app=layerslider|sendto@Drupal.layerslider_video_path', '', 'width=480,height=500,resizable=1');
        return false;
      });
      // edit css file
      $(ls_edit_css, context).click(function () {
        window.open(cssfileBrowse, '', 'width=600,height=590,scrollbars=1');
        return false;
      });

      // change layer html text
      $(ls_text_box, context).live('keyup', function() {
        Drupal.layerslider_slide_layer_value(ls_window, this);
      });
      // change layer top position
      $(ls_top_pos, context).live('keyup', function() {
        Drupal.layerslider_slide_layer_css_attributes(ls_window, this, 'top');
      });
      // change layer left position
      $(ls_left_pos, context).live('keyup', function() {
        Drupal.layerslider_slide_layer_css_attributes(ls_window, this, 'left');
      });
      // add class at layer
      $(ls_class_list, context).live('change', function() {
        Drupal.layerslider_slide_layer_class_attributes(ls_window, this);
      });

      // Effect setting for layers
      $(layer_effect_setting_box, context).each(function () {
        $('.vertical-tabs').find(this).live('change keyup', function(event) {
          var selected_element = $(ls_window).find(".selected");
          Drupal.layerslider_slide_layer_effects(selected_element, this, event);
        });
      });

      // Animation preview
      $(ls_animation_preview, context).click(function () {
        if($(this).html() == 'Stop Animation') {
          $('#layerslider-animation-preview').remove();
          $('.layerslider').show();
          $('.preventcover').remove();
          $(this).html('Animation Preview');
        }
        else {
          $('.layerslider').clone().attr('id','layerslider-animation-preview').removeAttr('class').appendTo('#edit-layerslider-slide-preview');
          $('#layerslider-animation-preview .ls-layer').clone().appendTo('#layerslider-animation-preview');
          $('#layerslider-animation-preview').layerSlider({
            animateFirstLayer : true
          });
          $('.layerslider').hide();
          $("#edit-layerslider-add-layer").append('<div class="preventcover" style="position: absolute;top:0;left:0;width: 100%;height:100%;z-index:2;"></div>');
          $("#edit-layerslider-slide-layer").append('<div class="preventcover" style="position: absolute;top:0;left:0;width: 100%;height:100%;z-index:2;"></div>');
          $(this).html('Stop Animation');
        }
        return false;
      });

    }
  };



  // --- convert color rgb into hexadecimal 
  Drupal.colorToHex = function(color) {
    if (color.substr(0, 1) === '#') {
        return color;
    }
    var digits = /(.*?)rgb\((\d+), (\d+), (\d+)\)/.exec(color);

    var red = parseInt(digits[2]);
    var green = parseInt(digits[3]);
    var blue = parseInt(digits[4]);

    var rgb = blue | (green << 8) | (red << 16);
    return digits[1] + '#' + rgb.toString(16);
  };

  // --- set background from slidebg box to slide
  Drupal.set_slide_bg = function(container, box) {
    var slide = $(layerslider).find(ls_window);
    $(box).each(function () {
      var element = $(container).find(this);
      var element_name = element[0].name;

      if(element_name == 'background-color') {
        $(element).live('focusout', function() {
          var bg_value = $(element).val();
          if ((/^#([a-f0-9]{3}){1,2}$/i.exec(bg_value) == null) && bg_value != 'transparent') { // check input box value is valid color
            alert('Must be a valid hexadecimal CSS color value.');
          }
          else {
            slide.css(element_name, bg_value); // add updated attribute value
            if($(element).parent().find('.remove').length == 0 && bg_value != 'transparent') { 
              $(element).parent().append("<a href='#' class='remove'>Remove</a>");  // add background remove link
            }
          }
        });
      }
      else if(element_name == 'background-image') {
        $(element).live('focusin', function() {
          var bg_value = $(element).val();
          if(bg_value != 'none') {
            slide.css(element_name, 'url('+bg_value+')'); // add/update attribute value
          }
          if($(element).parent().find('.remove').length == 0 && bg_value != 'none') {
            $(element).parent().append("<a href='#' class='remove'>Remove</a>");  // add background remove link
          }
        });
      }
      else if(element_name == 'background-repeat') {
        $(element).live('change', function() {
          var bg_value = $(element).val();
          slide.css(element_name, bg_value); // add/update attribute value
        });
      }
      else {
        $(element).live('change', function() {
          var bg_value = $(element).val();
          slide.css(element_name, bg_value); // add/update attribute value
        });
      }
    });
  }

  // --- get background back from slide to slidebg box
  Drupal.get_slide_bg = function(container, box) {
    var slide = $(layerslider).find(ls_window);
    $(box).each(function () {
      var element = $(container).find(this);
      var element_name = element[0].name;
      var bg_value = slide.css(element[0].name);

      if(element_name == 'background-color') {
        if(bg_value != 'transparent') {
          element.val(Drupal.colorToHex(bg_value));
          $(element).parent().append("<a href='#' class='remove'>Remove</a>");  // add background remove link
        }
      }
      else if(element_name == 'background-image'){
        if(bg_value != 'none') {
          bg_value = bg_value.replace('url(','').replace(')','');
          element.val(bg_value);
          $(element).parent().append("<a href='#' class='remove'>Remove</a>");  // add background remove link
        }
      }
      else if(element_name == 'background-repeat'){
        element.val(bg_value);
      }
      else {
        element.val(bg_value);
      }
    });
  }

  // --- remove slide css attribues like background color, background image and set default value
  Drupal.layerslider_slide_css_attributes_remove = function(element) {
    var slide = $(layerslider).find(ls_window);
    var element_name = $(element).parent().find('.form-text')[0].name;
    var defaultValue = $(element).parent().find('.form-text').attr('defaultValue');

    $(slide).css(element_name, defaultValue);
    $(element).parent().find('.form-text').val(defaultValue);
    $(element).remove();
    return false;
  }

  // --- get effect back from slide to slide effect box
  Drupal.get_slide_effects = function(container, box) {
    var rel_effects_slide = $(layerslider).find(ls_window).attr('rel');
    $(box).each(function () {
      var element = $(container).find(this);
      element.val('');
      if(rel_effects_slide != undefined) { 
        var effect_keypair = rel_effects_slide.split(';');
        $(effect_keypair).each(function () {
          effect_name = this.split(':')[0].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
          effect_value = this.split(':')[1].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
          if(element[0].name == effect_name) {
            element.val(effect_value);
          };
        });
      }
    });
  }

  // --- get css from layerslider css window to parent window
  Drupal.layerslider_get_css = function() {
    var doc = window.opener.document,
        class_list = doc.getElementById("edit-class-list"),
        head       = doc.getElementsByTagName('head'),
        cssfile    = doc.getElementById("layerslider_slide_css"),
        randomNum  = Math.random();

    class_list.innerHTML = document.getElementById("edit-class-list").innerHTML;
    $(cssfile).remove();
    $(head).append('<link rel="stylesheet" type="text/css" href="'+cssfile.href+'?'+randomNum+'" rel="stylesheet" id="layerslider_slide_css"/>');
    return false;
  }

  // --- get image path from imce window
  Drupal.layerslider_image_path = function(imagedata) {
    Drupal.layerslider_slide_layer_add(ls_window, 'image', imagedata);
  }

  // --- get videodata from video window
  Drupal.layerslider_video_path = function(videodata) {
    Drupal.layerslider_slide_layer_add(ls_window, 'video', videodata);
  }

  // --- add layers
  Drupal.layerslider_slide_layer_add = function(container, element_type, element_value) {

    var element = $('<div></div>')[0];
    if(element_type =='image') {
      $(element).append(element_value);  // image
    }
    else if(element_type =='video') {
      $(element).append(element_value);  // video
    }
    else {
      element.textContent = element_value;  // default text of layer
    }

    element.style.position = "absolute";  // set postion of layer
    element.style.top = "0"; // initilize top position
    element.style.left = "0"; // initilize left position
    
    var layerId = $(ls_window).children().length+1; // initilize layerId
    elementIdName = 'layer_'+layerId;
    elementclassName = 'ls-s'+layerId;

    // get unique layerId
    while($(container).find('#'+elementIdName).length > 0) {
      layerId++;
      elementIdName = 'layer_'+layerId;
      elementclassName = 'ls-s'+layerId;
    }
   
    element.setAttribute('id',elementIdName); // set layerId for layer
    element.setAttribute('class',elementclassName);  // set class for layer

    $(element).mouseup(function () {
      Drupal.layerslider_slide_layer_selected(container, $(this).attr( 'id' )); // bind event with newly added layer
    });

    // bind draggable event with newly added layer
    $(element).draggable({
      containment: container,
      cursor: "move",
    });

    $(container).append(element); // add layer at preview window
    Drupal.layerslider_slide_layer_list(ls_window, ls_list_box); // add new added layer at layer list box
  };

  // --- delete layers
  Drupal.layerslider_slide_layer_delete = function(elementIdName) {
    if(confirm('Are you sure you want to delete this layer?')) {
      $(ls_window).find(elementIdName).remove();
      Drupal.layerslider_slide_layer_list(ls_window, ls_list_box);
    }
  }

  // --- list layers
  Drupal.layerslider_slide_layer_list = function(container, layer_list_container) {

    var layers = "<ul id='sortable'>";
    $(container).children().each(function () {
      //get the id of layer and check the selected layer
      if($(this).hasClass( 'selected' )){ var layerid = " id =" +$(this).attr( 'id' )+ " class= 'selected'"; }
      else { var layerid = " id="+$(this).attr( 'id' ); }

      //create list of layers
      var layer_text = $(this).html().replace(/</g, "&lt;").replace(/>/g, "&gt;");
      var layer = "<li"+ layerid +"><a class='layerslider_delete_layer' href=\"javascript:Drupal.layerslider_slide_layer_delete('#" + $(this).attr( 'id' ) + "')\">[Delete]</a><span>"+ layer_text +"</span></li>";
      layers += layer;
    });
    layers += '</ul>';

    $(layer_list_container).html(layers);
    $( "#sortable" ).sortable({
      start: function(event, ui) {
        prevPagesOrder = $(this).sortable('toArray');
      },
      update: function(event, ui) {
        var currentOrder = $(this).sortable('toArray');
        var first = ui.item[0].id;
        var second = prevPagesOrder[currentOrder.indexOf(first)];
        var first_index = currentOrder.indexOf(first);
        var second_index = currentOrder.indexOf(second);

        // change the order of layers
        if(first_index > second_index){ $("#"+first).insertAfter("#"+second); }
        else { $("#"+first).insertBefore("#"+second); }
      }
    });

    $('#sortable').children().click(function () {
      Drupal.layerslider_slide_layer_selected(container, $(this).attr( 'id' )); // get layer selected at preview window
    });
  };

  // --- select layers
  Drupal.layerslider_slide_layer_selected = function(container, element) {

    var element = '#'+element;
    var top = parseInt($(element).position().top); // selected layer top position
    var left = parseInt($(element).position().left); // selected layer left position
    var textvalue = $(element).html(); // html text of selected layer
    var classname = $(element).attr('class').split(' ')[2]; // class of selected layer
    var rel_effects = $(element).attr('rel');

    $(container).children().removeClass("selected"); // remove selected class form all the child element
    $(container).find(element).addClass("selected"); // add selected class with selected layer

    $(ls_text_box).val(textvalue);
    $(ls_top_pos).val(top);
    $(ls_left_pos).val(left);
    $(ls_class_list).val('.'+classname);


    // assign effect value back to layer Effect box from selected layers
    $(layer_effect_setting_box).each(function () {
      var element = $('.vertical-tabs').find(this);
      element.val('');
      if(rel_effects != undefined) { 
        var effect_keypair = rel_effects.split(';');
        $(effect_keypair).each(function () {
          effect_name = this.split(':')[0].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
          effect_value = this.split(':')[1].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
          if(element[0].name == effect_name) {
            element.val(effect_value);
          };
        });
      }
    });

    Drupal.layerslider_slide_layer_list(container, ls_list_box); // update layer list box
  };

  // --- update layer value
  Drupal.layerslider_slide_layer_value = function(container, element) {
    var textvalue = $(element).val();
    if(textvalue.length == 0){ $(container).find(".selected").html('Enter Text'); } // check if input box no value, layer will be assigned default text('Enter Text')
    else { $(container).find(".selected").html(textvalue); } // find selected layer and assign input box value

    Drupal.layerslider_slide_layer_list(container, ls_list_box);  // update layer list box
    return false;
  };

  // --- update layer attributes like top, left
  Drupal.layerslider_slide_layer_css_attributes = function(container, element, attribute) {
    var attribute_value = parseInt($(element).val());
    var selected_element = $(container).find(".selected");

    if(isNaN(attribute_value) && $(element).val().length != 0) { alert('Enter number only.'); } // check input box value is number
    else { selected_element.css(attribute, attribute_value); } // find selected layer and add updated attribute value
    return false;
  };

  // --- update layer attributes class
  Drupal.layerslider_slide_layer_class_attributes = function(container, class_list_box) {
    var attribute_value = (class_list_box.value).slice( 1 ); // get the changed class name from the class list box
    var selected_element = $(container).find(".selected"); // find selected layer
    var classname = $(selected_element).attr('class').split(' ')[2]; // get class of selected layer
    if(attribute_value.length == 0) {
      selected_element.removeClass(classname); // remove class at selected layer
    }
    else {
      selected_element.removeClass(classname); // remove class at selected layer
      selected_element.removeClass('selected'); // remove 'selected' class from selected layer
      selected_element.addClass(attribute_value +' selected'); // add class at selected layer
    }
    return false;
  };

  // --- Effects
  Drupal.layerslider_slide_layer_effects = function(selected_element, effect_box, event) {

    var rel_effects = $(selected_element).attr('rel');
    var effects     = new Array();
    var effect_list = '';
    var i = 1;

    // validate text box value
    if(event.type == 'keyup') {
      // check input box value is number
      if((isNaN(parseInt($(effect_box).val())) || parseInt($(effect_box).val()) != $(effect_box).val()) && $(effect_box).val().length != 0) {
        alert('Enter number only.');
        $(effect_box).val('');
        var new_effects = '';
      }
      else { var new_effects = $(effect_box).val(); }
    }
    else { var new_effects = $(effect_box).val(); }

    //create array of rel value and assign new one;
    if(rel_effects != undefined) { 
      var effect_keypair = rel_effects.split(';');
      $(effect_keypair).each(function () {
        effect_name = this.split(':')[0].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
        effect_value = this.split(':')[1].replace(/(^[\s]+|[\s]+$)/g, ''); // remove space
        if(effect_box.name == effect_name) {
          effect_value = new_effects;
        }
        else { effects[effect_box.name] = new_effects; }
        effects[effect_name] = effect_value;
      });
    }
    else { effects[effect_box.name] = new_effects; }

    // create effect list with new one for rel value
    for (effect_name in effects) {
      if(effects[effect_name].length != 0) {
        if(i > 1){ effect_list += '; '; }
        effect_list += effect_name + ': ' + effects[effect_name];
        i++;
      }
    }

    if(effect_list.length != 0) { selected_element.attr('rel', effect_list); } // set effects for layer with rel tag
    else { selected_element.removeAttr('rel'); }  // remove rel tag from the layers

    return false;
  };

})(jQuery);

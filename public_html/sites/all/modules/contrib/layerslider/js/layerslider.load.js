(function ($) {

  // Behavior to load layerslider
  Drupal.behaviors.layerslider = {
    attach: function(context, settings) {
      $('.layerslider', context).once('layerslider', function() {
        $(this).each(function() {
          var $this = $(this);
          var id = $this.attr('id');
          if (settings.layerslider !== undefined) {
            var optionset = settings.layerslider.instances[id];
            if (optionset) {
              $this.layerSlider(settings.layerslider.optionsets[optionset]);
            }
            else {
              $this.layerSlider();
            }
          }
        }); 
      });
    }
  };

}(jQuery));

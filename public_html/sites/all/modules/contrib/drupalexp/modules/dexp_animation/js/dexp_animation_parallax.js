/*!Copyright (c) 2014 Drual Exp (http://drupalexp.com)*/

jQuery(document).ready(function($) {
  var speedFactor = 0.2;
  var window_height = $(window).height();
  $(window).bind('resize',function(){
    window_height = $(window).height();
  });
  $('.dexp-parallax').each(function() {
    var $this = $(this),element_h=$this.outerHeight(),background_h=$this.data('background-height');
    if(background_h < (element_h+window_height*speedFactor)){
      background_h = element_h+window_height*speedFactor;
      $this.css({
        'backgroundSize': 'auto '+ background_h +'px'
      });
    }
    $(window).bind('scroll', function() {
      if($(window).width()<768) return;
      var delta = Math.round(isappears($this) * speedFactor);
      if (delta > 0) {
        var background_y = $this.outerHeight() - background_h + delta;
        $this.css({
          backgroundPosition:'50% '+background_y+'px'
        });
      }
    });
  });

  function isappears(element) {
    var scrolltop = $(window).scrollTop();
    if ((scrolltop + window_height) >= ($(element).offset().top)){// && scrolltop <= ($(element).offset().top + $(element).outerHeight())) {
      return scrolltop + window_height - $(element).offset().top;
    }
    return 0;
  }
});
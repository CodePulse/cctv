jQuery(document).ready(function($){
  $(".dexp-animate").each(function(){
    var $this = $(this);
    var animate_class = $this.data('animate');
    $this.appear(function(){
      setTimeout(function(){
        $this.addClass(animate_class + ' animated');
        $this.addClass('dexp-animate-visible');
      },200);
    },{
      accX: 0,
      accY: 0,
      one:false
    });	
  });
})
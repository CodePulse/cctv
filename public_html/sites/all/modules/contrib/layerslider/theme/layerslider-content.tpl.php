<?php
/**
 * @file
 * Default output for a Layerslider.
*/
?>
<div class="layerslider-wrapper">
  <div class="layerslider" id="layerslider-<?php print $id; ?>" style="margin: 0px auto;<?php print $width . $height . $bgcolor . $bgimage; ?>">
    <?php foreach($items as $item) : ?>
      <?php print render($item); ?>
    <?php endforeach; ?>
  </div>
</div>
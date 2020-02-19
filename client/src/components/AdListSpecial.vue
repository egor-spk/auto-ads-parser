<template>
  <q-virtual-scroll
    :items="ads"
    :virtual-scroll-item-size="150"
    :virtual-scroll-sticky-size-start="50"
    separator
  >
    <template v-slot="{ item, index }">
     <AdItemSpecial :key="index" :ad="item" :type="type"/>
    </template>
  </q-virtual-scroll>
</template>

<script>
import AdItemSpecial from './AdItemSpecial'

export default {
  name: 'AdListSpecial',
  components: { AdItemSpecial },
  props: {
    ads: Array,
    type: String
  }
}
</script>

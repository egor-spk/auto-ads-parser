<template>
  <q-virtual-scroll
    :items="ads"
    :virtual-scroll-item-size="150"
    :virtual-scroll-sticky-size-start="50"
    :virtual-scroll-slice-size="15"
    separator
  >
    <template v-slot="{ item, index }">
     <AdItem :key="index" :ad="item"/>
    </template>
  </q-virtual-scroll>
</template>

<script>
import AdItem from './AdItem'

export default {
  name: 'AdList',
  components: { AdItem },
  props: {
    ads: Array
  }
}
</script>

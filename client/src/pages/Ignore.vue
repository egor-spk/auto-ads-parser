<template>
  <q-page>
    <AdListSpecial :ads="ignore" :remove="'ignore'" />
  </q-page>
</template>

<script>
import { mapGetters } from 'vuex'
import AdListSpecial from '../components/AdListSpecial'

export default {
  name: 'Ignore',
  components: { AdListSpecial },
  computed: {
    ...mapGetters(['ignore'])
  }
}
</script>

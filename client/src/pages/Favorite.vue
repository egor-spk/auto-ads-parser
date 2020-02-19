<template>
  <q-page>
    <AdListSpecial :ads="favorite" :type="'favorite'"/>
  </q-page>
</template>

<script>
import { mapGetters } from 'vuex'
import AdListSpecial from '../components/AdListSpecial'

export default {
  name: 'Favorite',
  components: { AdListSpecial },
  computed: {
    ...mapGetters(['favorite'])
  }
}
</script>

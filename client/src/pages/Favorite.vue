<template>
  <q-page>
    <AdListSpecial :ads="favorite" :remove="removeFromFavorite"/>
  </q-page>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex'
import AdListSpecial from '../components/AdListSpecial'

export default {
  name: 'Favorite',
  components: { AdListSpecial },
  computed: {
    ...mapGetters(['favorite'])
  },
  methods: {
    ...mapMutations(['removeFromFavorite'])
  }
}
</script>

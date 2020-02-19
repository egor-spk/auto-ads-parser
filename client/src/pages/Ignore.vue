<template>
  <q-page>
    <AdListSpecial :ads="ignore" :remove="removeFromIgnore" />
  </q-page>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex'
import AdListSpecial from '../components/AdListSpecial'

export default {
  name: 'Ignore',
  components: { AdListSpecial },
  computed: {
    ...mapGetters(['ignore'])
  },
  methods: {
    ...mapMutations(['removeFromIgnore'])
  }
}
</script>

<style lang="scss" scoped>
  .card {
    height: 150px;
    padding: 8px 0 8px 15px;
  }

  .q-layout-padding {
    padding: 0 15px 0 15px
  }

  .buttons {
    margin-top: auto;
  }

  .images {
    width: 65%;
  }
</style>

<style>
  .images-carousel > .q-carousel__navigation--bottom {
    bottom: 0;
  }
</style>

<template>
  <q-page>
    <q-pull-to-refresh @refresh="refreshAds">
      <AdList :ads="items"/>
    </q-pull-to-refresh>

    <q-page-sticky position="bottom-right" :offset="[12, 12]">
      <q-fab direction="up" icon="list" color="primary">
        <q-fab-action :outline="!(itemsType === 'all')" @click="itemsType = 'all'" external-label label-position="left"
                      color="primary" icon="font_download" label="Все"/>
        <q-fab-action :outline="!(itemsType === 'autoru')" @click="itemsType = 'autoru'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Auto.ru"/>
        <q-fab-action :outline="!(itemsType === 'avito')" @click="itemsType = 'avito'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Avito"/>
      </q-fab>
    </q-page-sticky>
  </q-page>
</template>

<script>
import { mapActions, mapGetters } from 'vuex'
import AdList from '../components/AdList'

export default {
  name: 'Ads',
  components: { AdList },
  data () {
    return {
      itemsType: 'all'
    }
  },
  methods: {
    ...mapActions(['fetchAds']),
    async refreshAds (done) {
      await this.fetchAds()
      done()
    }
  },
  computed: {
    ...mapGetters(['ads', 'avitoAds', 'autoRuAds']),
    itemsSrc () {
      switch (this.itemsType) {
        case 'avito':
          return this.avitoAds
        case 'autoru':
          return this.autoRuAds
        default:
          return this.ads
      }
    },
    items () {
      return this.itemsSrc
    }
  }
}
</script>

<template>
  <q-page>
    <AdList :ads="items"/>

    <q-page-sticky position="bottom-right" :offset="[12, 12]">
      <q-fab direction="up" icon="list" color="primary">
        <q-fab-action :outline="!(itemsType === 'all')" @click="itemsType = 'all'" external-label label-position="left"
                      color="primary" icon="font_download" label="Все">
          <q-badge :color="ads.length ? 'positive' : 'negative'" floating>{{ads.length}}</q-badge>
        </q-fab-action>
        <q-fab-action :outline="!(itemsType === 'autoru')" @click="itemsType = 'autoru'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Auto.ru">
          <q-badge :color="autoRuAds.length ? 'positive' : 'negative'" floating>{{autoRuAds.length}}</q-badge>
        </q-fab-action>
        <q-fab-action :outline="!(itemsType === 'avito')" @click="itemsType = 'avito'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Avito">
          <q-badge :color="avitoAds.length ? 'positive' : 'negative'" floating>{{avitoAds.length}}</q-badge>
        </q-fab-action>
      </q-fab>
    </q-page-sticky>
  </q-page>
</template>

<script>
import { mapGetters } from 'vuex'
import AdList from '../components/AdList'

export default {
  name: 'Ads',
  components: { AdList },
  data () {
    return {
      itemsType: 'all'
    }
  },
  computed: {
    ...mapGetters(['ads', 'avitoAds', 'autoRuAds']),
    items () {
      switch (this.itemsType) {
        case 'avito':
          return this.avitoAds
        case 'autoru':
          return this.autoRuAds
        default:
          return this.ads
      }
    }
  }
}
</script>

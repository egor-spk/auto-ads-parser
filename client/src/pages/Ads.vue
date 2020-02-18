<template>
  <q-page>
    <q-tabs
      v-model="tab"
      inline-label
      class="bg-primary text-white shadow-2"
    >
      <q-tab name="all" label="Все"/>
      <q-tab name="autoru" label="Auto.ru"/>
      <q-tab name="avito" label="Avito"/>
    </q-tabs>
    <q-tab-panels
      v-model="tab"
      keep-alive
      swipeable
    >
      <q-tab-panel name="all">
          <AdList :ads="ads"/>
      </q-tab-panel>

      <q-tab-panel name="autoru">
          <AdList :ads="autoRuAds"/>
      </q-tab-panel>

      <q-tab-panel name="avito">
          <AdList :ads="avitoAds"/>
      </q-tab-panel>
    </q-tab-panels>
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
      tab: 'all'
    }
  },
  computed: {
    ...mapGetters(['ads', 'avitoAds', 'autoRuAds'])
  }
}
</script>

<style scoped>
  .q-tab-panel {
    padding: 0;
  }

  .q-tab-panels {
    height: calc(100vh - 98px);
  }
</style>

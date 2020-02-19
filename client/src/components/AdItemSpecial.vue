<template>
  <q-item class="card">
    <q-item-section thumbnail class="images">
      <q-img :src="ad.images[0]" />
    </q-item-section>
    <q-item-section top no-wrap class="description column">
      <q-badge outline color="primary" class="justify-center">{{ ad.type }}</q-badge>
      <div>Пробег: {{ad.mileage}}</div>
      <div>Год: {{ad.year}}</div>
      <div>Цена: {{ad.price}}</div>
      <div class="buttons row justify-between">
        <q-btn :ripple="false" round outline color="negative" size="sm" @click="onRemove(ad.id)" icon="remove" />
        <q-btn round outline color="primary" size="sm" @click="open(ad.link)" icon="open_in_new" />
      </div>
    </q-item-section>
  </q-item>
</template>

<script>

import { mapMutations } from 'vuex'

export default {
  name: 'AdItemSpecial',
  props: {
    ad: Object,
    type: String
  },
  methods: {
    ...mapMutations(['removeFrom']),
    onRemove (id) {
      this.removeFrom({ id: id, type: this.type })
    },
    open (link) {
      window.open(link, '_system')
    }
  }
}
</script>

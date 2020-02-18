<template>
  <q-item class="card">
    <q-item-section thumbnail class="images">
      <q-img :src="ad.images[0]"/>
    </q-item-section>
    <q-item-section top no-wrap class="description column">
      <div>Пробег: {{ad.mileage}}</div>
      <div>Год: {{ad.year}}</div>
      <div>Цена: {{ad.price}}</div>
      <div class="buttons row justify-between">
        <q-btn round outline color="negative" size="sm" @click="ignore(ad.id)" icon="thumb_down"/>
        <q-btn round outline color="positive" size="sm" @click="favorite(ad.id)" icon="favorite_border"/>
        <q-btn round outline color="primary" size="sm" @click="open(ad.link)" icon="open_in_new"/>
      </div>
    </q-item-section>
  </q-item>
</template>

<script>
import { notifyError } from '../helpers/notify'

export default {
  name: 'AdItem',
  props: {
    ad: Object
  },
  data () {
    return {
      slide: 0
    }
  },
  methods: {
    favorite (id) {
      notifyError('Not implemented')
    },
    ignore (id) {
      notifyError('Not implemented')
    },
    open (link) {
      window.open(link, '_system')
    }
  }
}
</script>

<style lang="scss" scoped>
  .card {
    height: 150px;
    padding: 8px 0 8px 16px;
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

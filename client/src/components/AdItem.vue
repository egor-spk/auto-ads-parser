<template>
  <q-item class="card">
    <q-item-section thumbnail class="images">
      <q-img :src="ad.images[0]" />
    </q-item-section>
    <q-item-section top no-wrap class="description column">
      <div>Пробег: {{ad.mileage}}</div>
      <div>Год: {{ad.year}}</div>
      <div>Цена: {{ad.price}}</div>
      <div class="buttons row justify-between">
        <q-btn :ripple="false" round outline color="negative" size="sm" @click="onIgnore(ad.id)" icon="thumb_down" />
        <q-btn
          round
          outline
          color="positive"
          size="sm"
          @click="onFavorite(ad.id)"
          :icon="favorite.includes(ad.id) ? 'favorite' : 'favorite_border'"
        />
        <q-btn round outline color="primary" size="sm" @click="open(ad.link)" icon="open_in_new" />
      </div>
    </q-item-section>
  </q-item>
</template>

<script>
import { mapMutations, mapState } from 'vuex'

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
  computed: {
    ...mapState(['favorite'])
  },
  methods: {
    ...mapMutations(['addFavorite', 'removeFrom', 'addIgnore']),
    onFavorite (id) {
      if (this.favorite.includes(id)) {
        this.removeFrom({ id: id, type: 'favorite' })
      } else {
        this.addFavorite(id)
      }
    },
    onIgnore (id) {
      this.addIgnore(id)
    },
    open (link) {
      window.open(link, '_system')
    }
  }
}
</script>

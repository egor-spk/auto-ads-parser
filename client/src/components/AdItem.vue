<template>
  <q-item class="card">
    <q-item-section thumbnail class="images">
      <q-carousel
        class="images-carousel"
        transition-prev="slide-right"
        transition-next="slide-left"
        animated
        v-model="slide"
        infinite
        navigation
        swipeable>

        <q-carousel-slide class="image" v-for="(item, index) in ad.images" :key="index" :name="index" :img-src="item"/>

      </q-carousel>
    </q-item-section>
    <q-item-section class="description">
      <p>Пробег: {{ad.mileage}}</p>
      <p>Год: {{ad.year}}</p>
      <p>Цена: {{ad.price}}</p>
      <p class="buttons row justify-end">
        <q-btn round outline color="primary" @click="favorite(ad.id)" icon="favorite_border"/>
        <q-btn round outline color="primary" @click="open(ad.link)" icon="open_in_new"/>
      </p>
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
    open (link) {
      window.open(link, '_system')
    }
  }
}
</script>

<style lang="scss" scoped>
  .card {
    height: 170px;
    padding: 8px 0 8px 16px;
  }

  .images {
    width: 50%;
  }

  .images-carousel {
    width: 100%;
  }

  .image {
    width: 100%;
  }

  .description {
    > .buttons {
      margin-bottom: 0;
    }

    .q-btn {
      margin: 0 10px 0 10px;
    }

    .q-btn:last-child {
      margin: 0 0 0 5px;
    }
  }
</style>

<style>
  .images-carousel > .q-carousel__navigation--bottom {
    bottom: 0;
  }
</style>

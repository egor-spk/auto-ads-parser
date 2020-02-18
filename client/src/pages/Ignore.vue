<template>
  <q-page padding>
    <q-list padding separator>
      <q-item class="card" v-for="(ad, index) in ignore" :key="index">
        <q-item-section thumbnail class="images">
          <q-img :src="ad.images[0]" />
        </q-item-section>
        <q-item-section top no-wrap class="description column">
          <q-badge outline color="primary" class="justify-center">{{ ad.type }}</q-badge>
          <br>
          <div>Пробег: {{ad.mileage}}</div>
          <div>Год: {{ad.year}}</div>
          <div>Цена: {{ad.price}}</div>
          <div class="buttons row justify-end">
            <q-btn round outline color="primary" size="sm" @click="remove(ad.id)" icon="remove" />
          </div>
        </q-item-section>
      </q-item>
    </q-list>
  </q-page>
</template>

<script>
import { mapGetters, mapMutations } from 'vuex'

export default {
  name: 'Ignore',
  computed: {
    ...mapGetters(['ignore'])
  },
  methods: {
    ...mapMutations(['removeFromIgnore']),
    remove (id) {
      this.removeFromIgnore(id)
    }
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

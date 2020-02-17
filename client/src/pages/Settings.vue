<template>
  <q-page>
    <q-list padding>
      <q-item-label header>Сервер</q-item-label>
      <q-item>
        <q-input v-model="address"
                 style="width: 100%"
                 ref="address"
                 label="Адрес парсера"
                 type="url"
                 debounce="500"
                 :rules="[val => isValidUrl(val) || 'Адрес парсера должен быть валидной ссылкой']">
          <template v-slot:prepend>
            <q-icon name="cloud_queue"/>
          </template>
          <template v-slot:append>
            <q-icon name="close" @click="address = ''" class="cursor-pointer"/>
          </template>
        </q-input>
      </q-item>
    </q-list>
  </q-page>
</template>

<script>
import { mapMutations, mapState } from 'vuex'

export default {
  name: 'Settings',
  data () {
    return {
      address: ''
    }
  },
  methods: {
    ...mapMutations(['setSettings']),
    isValidUrl (url) {
      return /^(http|https):\/\/[^ "]+$/.test(url)
    }
  },
  mounted () {
    this.address = this.settings.address
  },
  computed: mapState(['settings']),
  watch: {
    address (val) {
      if (this.isValidUrl(val)) {
        this.setSettings({ address: val })
      }
    }
  }
}
</script>

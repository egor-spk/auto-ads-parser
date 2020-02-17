import Vue from 'vue'
import Vuex from 'vuex'
import { LocalStorage } from 'quasar'
import { notifyError } from 'src/helpers/notify'

Vue.use(Vuex)

export default function () {
  const Store = new Vuex.Store({
    strict: process.env.DEV,
    state: {
      ads: [],
      settings: {}
    },
    mutations: {
      setAds (state, ads) {
        state.settings = ads
      },
      setSettings (state, settings) {
        LocalStorage.set('address', settings.address)
        state.settings = settings
      }
    },
    actions: {
      fetchSettings ({ commit }) {
        const settings = {
          address: ''
        }
        const address = LocalStorage.getItem('address')
        if (address !== null) {
          settings.address = address
        }

        commit('setSettings', settings)
      },
      async fetchAds ({ commit, state }) {
        if (state.settings.address) {
          try {
            const response = await fetch(state.settings.address)
            if (!response.ok) {
              throw Error(`${response.statusCode}: ${response.statusMessage}`)
            }
            const ads = await response.json()
            commit('setAds', ads)
          } catch (e) {
            notifyError(`Unable to fetch ads. ${e.message}`)
          }
        }
      },
      async fetchData ({ dispatch }) {
        await dispatch('fetchSettings')
        await dispatch('fetchAds')
      }
    }
  })

  return Store
}

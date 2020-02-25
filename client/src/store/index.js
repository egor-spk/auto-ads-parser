import Vue from 'vue'
import Vuex from 'vuex'
import { LocalStorage } from 'quasar'
import { notifyError } from 'src/helpers/notify'

Vue.use(Vuex)

export default function () {
  const Store = new Vuex.Store({
    strict: process.env.DEV,
    state: {
      ads: {},
      settings: {},
      favorite: [],
      ignore: []
    },
    mutations: {
      setAds (state, ads) {
        state.ads = ads
      },
      setSettings (state, settings) {
        LocalStorage.set('address', settings.address)
        state.settings = settings
      },
      addFavorite (state, id) {
        const arr = state.favorite
        const index = arr.indexOf(id)
        if (index === -1) {
          arr.push(id)
          state.favorite = arr
          LocalStorage.set('favorite', JSON.stringify(state.favorite))
        }
      },
      addIgnore (state, id) {
        const arr = state.ignore
        const index = arr.indexOf(id)
        if (index === -1) {
          arr.push(id)
          state.ignore = arr
          LocalStorage.set('ignore', JSON.stringify(state.ignore))
        }
      },
      removeFrom (state, payload) {
        let arr = []
        if (payload.type === 'favorite') {
          arr = state.favorite
        } else {
          arr = state.ignore
        }
        const index = arr.indexOf(payload.id)
        if (index > -1) {
          arr.splice(index, 1)
          if (payload.type === 'favorite') {
            state.favorite = arr
            LocalStorage.set('favorite', JSON.stringify(state.favorite))
          } else {
            state.ignore = arr
            LocalStorage.set('ignore', JSON.stringify(state.ignore))
          }
        }
      },
      setFavorite (state, favorite) {
        state.favorite = JSON.parse(favorite)
        LocalStorage.set('favorite', favorite)
      },
      setIgnore (state, ignore) {
        state.ignore = JSON.parse(ignore)
        LocalStorage.set('ignore', ignore)
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
      fetchFavorite ({ commit }) {
        const favorite = LocalStorage.getItem('favorite')
        if (favorite !== null) {
          commit('setFavorite', favorite)
        }
      },
      fetchIgnore ({ commit }) {
        const ignore = LocalStorage.getItem('ignore')
        if (ignore !== null) {
          commit('setIgnore', ignore)
        }
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
        await dispatch('fetchIgnore')
        await dispatch('fetchFavorite')
        await dispatch('fetchAds')
      }
    },
    getters: {
      autoRuAds (state, getters) {
        if ('autoru' in state.ads) {
          return state.ads.autoru.items.filter(ad => !state.ignore.includes(ad.id))
        }

        return []
      },
      avitoAds (state, getters) {
        if ('avito' in state.ads) {
          return state.ads.avito.items.filter(ad => !state.ignore.includes(ad.id))
        }

        return []
      },
      ads (state, getters) {
        return [...getters.autoRuAds, ...getters.avitoAds]
      },
      favorite (state) {
        let autoru = []
        let avito = []

        if ('autoru' in state.ads) {
          autoru = state.ads.autoru.items.filter(ad => state.favorite.includes(ad.id)).map(ad => {
            ad.type = 'auto.ru'
            return ad
          })
        }

        if ('avito' in state.ads) {
          avito = state.ads.avito.items.filter(ad => state.favorite.includes(ad.id)).map(ad => {
            ad.type = 'avito'
            return ad
          })
        }

        return [...autoru, ...avito]
      },
      ignore (state) {
        let autoru = []
        let avito = []

        if ('autoru' in state.ads) {
          autoru = state.ads.autoru.items.filter(ad => state.ignore.includes(ad.id)).map(ad => {
            ad.type = 'auto.ru'
            return ad
          })
        }

        if ('avito' in state.ads) {
          avito = state.ads.avito.items.filter(ad => state.ignore.includes(ad.id)).map(ad => {
            ad.type = 'avito'
            return ad
          })
        }

        return [...autoru, ...avito]
      }
    }
  })

  return Store
}

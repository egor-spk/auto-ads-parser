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
        state.favorite = favorite
        LocalStorage.set('favorite', JSON.stringify(favorite))
      },
      setIgnore (state, ignore) {
        state.ignore = ignore
        LocalStorage.set('ignore', JSON.stringify(ignore))
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
      fetchFavorite ({ commit, getters }) {
        let favorite = LocalStorage.getItem('favorite')
        if (favorite !== null) {
          favorite = JSON.parse(favorite)
          // оставляем только существующие объявления
          favorite = favorite.filter(favoriteId => {
            return getters.allAds.find(adItem => favoriteId === adItem.id) !== undefined
          })
          commit('setFavorite', favorite)
        }
      },
      fetchIgnore ({ commit, getters }) {
        let ignore = LocalStorage.getItem('ignore')
        if (ignore !== null) {
          ignore = JSON.parse(ignore)
          // оставляем только существующие объявления
          ignore = ignore.filter(ignoreId => {
            return getters.allAds.find(adItem => ignoreId === adItem.id) !== undefined
          })
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
        await dispatch('fetchAds')
        await dispatch('fetchIgnore')
        await dispatch('fetchFavorite')
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
      allAds (state) {
        let avito = [], autoru = []
        if ('avito' in state.ads) {
          avito = state.ads.avito.items
        }
        if ('autoru' in state.ads) {
          autoru = state.ads.autoru.items
        }
        return [...avito, ...autoru]
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
      },
      avitoDateTime (state) {
        if ('avito' in state.ads) {
          return state.ads.avito.date_time
        }
        return undefined
      },
      autoRuDateTime (state) {
        if ('autoru' in state.ads) {
          return state.ads.autoru.date_time
        }
        return undefined
      }
    }
  })

  return Store
}

<template>
  <q-page>
    <apexchart type="scatter" :height="height" width="100%" :series="items" :options="options"/>

    <q-page-sticky position="bottom-right" :offset="[12, 12]">
      <q-fab direction="up" icon="list" color="primary">
        <q-fab-action :outline="!(itemsType === 'all')" @click="itemsType = 'all'" external-label label-position="left"
                      color="primary" icon="font_download" label="Все"/>
        <q-fab-action :outline="!(itemsType === 'autoru')" @click="itemsType = 'autoru'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Auto.ru"/>
        <q-fab-action :outline="!(itemsType === 'avito')" @click="itemsType = 'avito'" external-label
                      label-position="left" color="primary" icon="directions_car" label="Avito"/>
      </q-fab>
    </q-page-sticky>
  </q-page>
</template>

<script>
import { mapGetters, mapState } from 'vuex'

export default {
  name: 'Charts',
  methods: {
    isPortait () {
      return screen.orientation.type.includes('portrait')
    }
  },
  data () {
    return {
      itemsType: 'all',
      height: this.isPortait() ? '90%' : '200%',
      isRender: true,
      min: 0,
      options: {
        chart: {
          toolbar: {
            show: false
          }
        },
        xaxis: {
          tickAmount: 10,
          title: {
            text: 'Цена, ₽'
          }
        },
        yaxis: {
          tickAmount: this.isPortait() ? 10 : 20,
          title: {
            text: 'Пробег, км'
          },
          min: min => this.min,
          max: max => this.max
        },
        tooltip: {
          x: {
            formatter: (val) => `${val} ₽`
          },
          y: {
            formatter: (val) => `${val} км`,
            title: {
              formatter: () => 'Пробег'
            }
          }
        }
      }
    }
  },
  computed: {
    ...mapGetters(['ads', 'avitoAds', 'autoRuAds']),
    ...mapState(['favorite']),
    items () {
      let source = []
      switch (this.itemsType) {
        case 'avito':
          source = this.avitoAds
          break
        case 'autoru':
          source = this.autoRuAds
          break
        default:
          source = this.ads
      }

      // находим минимальный пробег
      if (source.length) {
        const sortedAds = [...source].sort((a, b) => (a.mileage > b.mileage) ? 1 : ((b.mileage > a.mileage) ? -1 : 0))
        // eslint-disable-next-line vue/no-side-effects-in-computed-properties
        this.min = sortedAds[0].mileage
        // eslint-disable-next-line vue/no-side-effects-in-computed-properties
        this.max = sortedAds[sortedAds.length - 1].mileage
      }

      // разделяем обычные точки и избранные
      const favorite = [], other = []
      for (const ad of source) {
        if (!this.favorite.includes(ad.id)) {
          other.push([ad.price, ad.mileage])
        } else {
          favorite.push([ad.price, ad.mileage])
        }
      }

      return [{
        name: 'Обычные',
        data: other
      }, {
        name: 'Избранные',
        data: favorite
      }]
    }
  },
  created () {
    window.addEventListener('orientationchange', () => {
      const isPortait = this.isPortait()
      this.options.yaxis.tickAmount = isPortait ? 10 : 20
      this.height = isPortait ? '90%' : '100%'
    })
  }
}
</script>

<template>
  <q-virtual-scroll
    :items="items"
    :virtual-scroll-item-size="150"
    :virtual-scroll-sticky-size-start="50"
    :virtual-scroll-slice-size="15"
    separator
  >
    <template v-slot:before>
      <q-expansion-item class="shadow-1 rounded-borders" icon="sort_by_alpha" label="Сортировка">
        <q-splitter v-model="splitterModel">
          <template v-slot:before>
            <q-option-group
              :options="sortTypeOptions"
              type="radio"
              v-model="sortType"
            />
          </template>

          <template v-slot:after v-if="sortType !== 'none'">
            <q-option-group
              :options="sortDirectionOptions"
              type="radio"
              v-model="sortDirection"
            />
          </template>
        </q-splitter>
      </q-expansion-item>
      <br>
    </template>
    <template v-slot="{ item, index }">
      <AdItem :key="index" :ad="item"/>
    </template>
  </q-virtual-scroll>
</template>

<script>
import AdItem from './AdItem'

export default {
  name: 'AdList',
  components: { AdItem },
  data () {
    return {
      splitterModel: 50,
      sortType: 'none',
      sortDirection: 'desc',
      sortDirectionOptions: [
        {
          label: 'По убыванию',
          value: 'desc'
        },
        {
          label: 'По возрастанию',
          value: 'asc'
        }
      ],
      sortTypeOptions: [
        {
          label: 'Без',
          value: 'none'
        },
        {
          label: 'Цена',
          value: 'price'
        },
        {
          label: 'Пробег',
          value: 'mileage'
        },
        {
          label: 'Год',
          value: 'year'
        }
      ]
    }
  },
  props: {
    ads: Array
  },
  computed: {
    itemsSort () {
      switch (this.sortType) {
        case 'price':
          return [...this.ads].sort((a, b) => (a.price < b.price) ? 1 : ((b.price < a.price) ? -1 : 0))
        case 'mileage':
          return [...this.ads].sort((a, b) => (a.mileage < b.mileage) ? 1 : ((b.mileage < a.mileage) ? -1 : 0))
        case 'year':
          return [...this.ads].sort((a, b) => (a.year < b.year) ? 1 : ((b.year < a.year) ? -1 : 0))
        default:
          return this.ads
      }
    },
    items () {
      switch (this.sortDirection) {
        case 'asc':
          return [...this.itemsSort].reverse()
        default:
          return this.itemsSort
      }
    }
  }
}
</script>

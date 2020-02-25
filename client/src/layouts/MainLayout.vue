<template>
  <q-layout view="lHh Lpr lFf">
    <q-pull-to-refresh :disable="$route.path === '/settings'" @refresh="refreshAds">
      <q-header elevated>
        <q-toolbar>
          <q-btn
            flat
            dense
            round
            @click="leftDrawerOpen = !leftDrawerOpen"
            icon="menu"
            aria-label="Menu"
          />

          <q-toolbar-title>{{$route.meta.title}}</q-toolbar-title>

          <q-btn
            flat
            dense
            round
            @click="onFullScreen"
            :icon="isFullscreen ? 'fullscreen_exit' : 'fullscreen'"
            aria-label="Fullscreen"
          />
        </q-toolbar>
      </q-header>
    </q-pull-to-refresh>

    <q-drawer v-model="leftDrawerOpen" show-if-above>
      <q-img class="absolute-top" src="~assets/drawer-background.png" style="height: 150px">
        <div class="absolute-bottom bg-transparent">
          <q-avatar size="56px" class="q-mb-sm">
            <img src="~assets/app-logo-128x128.png"/>
          </q-avatar>
          <div class="text-weight-bold">Auto ads</div>
        </div>
      </q-img>

      <q-scroll-area
        style="height: calc(100% - 150px); margin-top: 150px; border-right: 1px solid #ddd"
      >
        <q-list padding>
          <q-item clickable v-ripple to="/" replace exact>
            <q-item-section avatar>
              <q-icon name="list_alt"/>
            </q-item-section>

            <q-item-section>Объявления</q-item-section>
          </q-item>

          <q-item clickable v-ripple to="/favorite">
            <q-item-section avatar>
              <q-icon name="favorite"/>
            </q-item-section>

            <q-item-section>Избранное</q-item-section>
          </q-item>

          <q-item clickable v-ripple to="/ignore">
            <q-item-section avatar>
              <q-icon name="thumb_down"/>
            </q-item-section>

            <q-item-section>Скрытые</q-item-section>
          </q-item>

          <q-item clickable v-ripple to="/charts">
            <q-item-section avatar>
              <q-icon name="bar_chart"/>
            </q-item-section>

            <q-item-section>Графики</q-item-section>
          </q-item>

          <q-separator/>

          <q-item clickable v-ripple to="/settings">
            <q-item-section avatar>
              <q-icon name="settings"/>
            </q-item-section>

            <q-item-section>Настройки</q-item-section>
          </q-item>

          <q-item clickable v-ripple to="/about">
            <q-item-section avatar>
              <q-icon name="info"/>
            </q-item-section>

            <q-item-section>О программе</q-item-section>
          </q-item>
        </q-list>
      </q-scroll-area>
    </q-drawer>

    <q-page-container>
        <router-view/>
    </q-page-container>
  </q-layout>
</template>

<script>
import { notifyError } from '../helpers/notify'
import { mapActions } from 'vuex'

export default {
  name: 'MainLayout',
  data () {
    return {
      leftDrawerOpen: false,
      isFullscreen: false
    }
  },
  methods: {
    onFullScreen () {
      if (this.$q.platform.is.cordova) {
        if (this.isFullscreen) {
          window.AndroidFullScreen.showSystemUI(
            () => {
              this.isFullscreen = false
            },
            error => {
              notifyError(error.message)
            }
          )
        } else {
          window.AndroidFullScreen.immersiveMode(
            () => {
              this.isFullscreen = true
            },
            error => {
              notifyError(error.message)
            }
          )
        }
      } else {
        notifyError('This feature only works for cordova android app')
      }
    },
    ...mapActions(['fetchAds']),
    async refreshAds (done) {
      await this.fetchAds()
      done()
    }
  }
}
</script>

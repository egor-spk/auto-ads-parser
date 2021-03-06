
const routes = [
  {
    path: '/',
    component: () => import('layouts/MainLayout.vue'),
    children: [
      { path: '', component: () => import('pages/Ads.vue'), meta: { title: 'Объявления' } },
      { path: 'charts', component: () => import('pages/Charts.vue'), meta: { title: 'Графики' } },
      { path: 'settings', component: () => import('pages/Settings.vue'), meta: { title: 'Настройки' } },
      { path: 'favorite', component: () => import('pages/Favorite.vue'), meta: { title: 'Избранное' } },
      { path: 'ignore', component: () => import('pages/Ignore.vue'), meta: { title: 'Скрытые' } },
      { path: 'about', component: () => import('pages/About.vue'), meta: { title: 'О программе' } }
    ]
  }
]

// Always leave this as last one
if (process.env.MODE !== 'ssr') {
  routes.push({
    path: '*',
    component: () => import('pages/Error404.vue')
  })
}

export default routes

import { Notify } from 'quasar'

export function notifyError (message) {
  Notify.create({
    progress: true,
    color: 'negative',
    message: message,
    icon: 'error_outline',
    timeout: 1000,
    actions: [
      { label: 'Close', color: 'white' }
    ]
  })
}

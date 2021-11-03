import {notification} from 'antd';

export type NotificationType = 'info' | 'success' | 'warning' | 'error';

// tslint:disable-next-line: no-any
export const openNotification = (title: string, content: string, type: NotificationType) => {
  notification.open({
    message: title,
    description: content,
    placement: 'bottomRight',
    type,
  });
};

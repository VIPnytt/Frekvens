import { Toast } from '../components/Toast';

export const name = 'Signal';

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case name.toLowerCase():
            toast(name, 30e3);
            break;
    }
};

export default name;

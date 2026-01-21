import { Toast } from "../components/Toast";

export const name = "Signal";

export const receiver = (json: { event?: string }) => {
    json?.event !== undefined && event(json.event);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case name.toLowerCase():
            toast(name, 30e3);
            break;
    }
};

from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import subprocess
import os
import json

app = Flask(__name__)
CORS(app)


CPP_EXECUTABLE =  r"C:\Users\Mr Magic\Desktop\Market\market\x64\Debug\market.exe"
  
DATA_FOLDER = "C:\\Users\\Mr Magic\\Desktop\\DSA Project\\"

class MarketplaceAPI:
    def __init__(self):
        self.sellers_file = os.path.join(DATA_FOLDER, "sellers.txt")
        self.customers_file = os.path.join(DATA_FOLDER, "customers.txt")
        self.products_file = os.path.join(DATA_FOLDER, "products.txt")
    
    def read_sellers(self):
        sellers = []
        try:
            with open(self.sellers_file, 'r') as f:
                for line in f:
                    parts = line.strip().split(',')
                    if len(parts) >= 5:
                        sellers.append({
                            'id': int(parts[0]),
                            'name': parts[1],
                            'email': parts[2],
                            'totalRating': float(parts[3]),
                            'ratingCount': int(parts[4]),
                            'rating': float(parts[3]) / int(parts[4]) if int(parts[4]) > 0 else 0
                        })
        except FileNotFoundError:
            pass
        return sellers
    
    def read_customers(self):
        customers = []
        try:
            with open(self.customers_file, 'r') as f:
                for line in f:
                    parts = line.strip().split(',')
                    if len(parts) >= 5:
                        customers.append({
                            'id': int(parts[0]),
                            'name': parts[1],
                            'email': parts[2],
                            'address': parts[3],
                            'phone': parts[4]
                        })
        except FileNotFoundError:
            pass
        return customers
    
    def read_products(self):
        products = []
        try:
            with open(self.products_file, 'r') as f:
                for line in f:
                    parts = line.strip().split(',')
                    if len(parts) >= 8:
                        products.append({
                            'id': int(parts[0]),
                            'sellerId': int(parts[1]),
                            'name': parts[2],
                            'price': float(parts[3]),
                            'category': parts[4],
                            'quantity': int(parts[5]),
                            'totalRating': float(parts[6]),
                            'ratingCount': int(parts[7]),
                            'rating': float(parts[6]) / int(parts[7]) if int(parts[7]) > 0 else 0
                        })
        except FileNotFoundError:
            pass
        return products
    
    def write_seller(self, seller):
        sellers = self.read_sellers()
        seller_id = max([s['id'] for s in sellers], default=0) + 1
        seller['id'] = seller_id
        seller['totalRating'] = 0
        seller['ratingCount'] = 0
        
        with open(self.sellers_file, 'a') as f:
            f.write(f"{seller_id},{seller['name']},{seller['email']},0,0\n")
        return seller
    
    def write_customer(self, customer):
        customers = self.read_customers()
        customer_id = max([c['id'] for c in customers], default=0) + 1
        customer['id'] = customer_id
        
        with open(self.customers_file, 'a') as f:
            f.write(f"{customer_id},{customer['name']},{customer['email']},{customer['address']},{customer['phone']}\n")
        return customer
    
    def write_product(self, product):
        products = self.read_products()
        product_id = max([p['id'] for p in products], default=0) + 1
        product['id'] = product_id
        product['totalRating'] = 0
        product['ratingCount'] = 0
        
        with open(self.products_file, 'a') as f:
            f.write(f"{product_id},{product['sellerId']},{product['name']},{product['price']},{product['category']},{product['quantity']},0,0\n")
        return product
    
    def update_product_rating(self, product_id, rating):
        products = self.read_products()
        for product in products:
            if product['id'] == product_id:
                product['totalRating'] += rating
                product['ratingCount'] += 1
                break
        
        with open(self.products_file, 'w') as f:
            for p in products:
                f.write(f"{p['id']},{p['sellerId']},{p['name']},{p['price']},{p['category']},{p['quantity']},{p['totalRating']},{p['ratingCount']}\n")
    
    def update_seller_rating(self, seller_id, rating):
        sellers = self.read_sellers()
        for seller in sellers:
            if seller['id'] == seller_id:
                seller['totalRating'] += rating
                seller['ratingCount'] += 1
                break
        
        with open(self.sellers_file, 'w') as f:
            for s in sellers:
                f.write(f"{s['id']},{s['name']},{s['email']},{s['totalRating']},{s['ratingCount']}\n")
    
    def update_product_quantity(self, product_id, quantity_change):
        products = self.read_products()
        for product in products:
            if product['id'] == product_id:
                product['quantity'] += quantity_change
                break
        
        with open(self.products_file, 'w') as f:
            for p in products:
                f.write(f"{p['id']},{p['sellerId']},{p['name']},{p['price']},{p['category']},{p['quantity']},{p['totalRating']},{p['ratingCount']}\n")

api = MarketplaceAPI()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/sellers/register', methods=['POST'])
def register_seller():
    data = request.json
    seller = api.write_seller(data)
    return jsonify(seller)

@app.route('/api/sellers/login', methods=['POST'])
def login_seller():
    data = request.json
    sellers = api.read_sellers()
    for seller in sellers:
        if seller['email'] == data['email']:
            return jsonify(seller)
    return jsonify({'error': 'Seller not found'}), 404

@app.route('/api/customers/register', methods=['POST'])
def register_customer():
    data = request.json
    customer = api.write_customer(data)
    return jsonify(customer)

@app.route('/api/customers/login', methods=['POST'])
def login_customer():
    data = request.json
    customers = api.read_customers()
    for customer in customers:
        if customer['email'] == data['email']:
            return jsonify(customer)
    return jsonify({'error': 'Customer not found'}), 404

@app.route('/api/products', methods=['GET'])
def get_products():
    products = api.read_products()
    return jsonify(products)

@app.route('/api/products/category/<category>', methods=['GET'])
def get_products_by_category(category):
    products = api.read_products()
    filtered = [p for p in products if p['category'].lower() == category.lower() and p['quantity'] > 0]
    return jsonify(filtered)

@app.route('/api/products/search', methods=['GET'])
def search_products():
    query = request.args.get('q', '').lower()
    products = api.read_products()
    filtered = [p for p in products if query in p['name'].lower() and p['quantity'] > 0]
    return jsonify(filtered)

@app.route('/api/products/top-rated', methods=['GET'])
def get_top_rated():
    products = api.read_products()
    sorted_products = sorted(products, key=lambda p: p['rating'], reverse=True)
    return jsonify(sorted_products[:5])

@app.route('/api/products/add', methods=['POST'])
def add_product():
    data = request.json
    product = api.write_product(data)
    return jsonify(product)

@app.route('/api/products/<int:product_id>/rate', methods=['POST'])
def rate_product(product_id):
    data = request.json
    rating = data.get('rating', 0)
    if 1 <= rating <= 5:
        api.update_product_rating(product_id, rating)
        return jsonify({'success': True})
    return jsonify({'error': 'Invalid rating'}), 400

@app.route('/api/sellers/<int:seller_id>/rate', methods=['POST'])
def rate_seller(seller_id):
    data = request.json
    rating = data.get('rating', 0)
    if 1 <= rating <= 5:
        api.update_seller_rating(seller_id, rating)
        return jsonify({'success': True})
    return jsonify({'error': 'Invalid rating'}), 400

@app.route('/api/checkout', methods=['POST'])
def checkout():
    data = request.json
    cart_items = data.get('items', [])
    
    total = 0
    for item in cart_items:
        product_id = item['productId']
        quantity = item['quantity']
        products = api.read_products()
        
        for product in products:
            if product['id'] == product_id:
                total += product['price'] * quantity
                api.update_product_quantity(product_id, -quantity)
                break
    
    return jsonify({'total': total, 'success': True})

if __name__ == '__main__':
   
    os.makedirs(DATA_FOLDER, exist_ok=True)
    app.run(debug=True, port=5000)